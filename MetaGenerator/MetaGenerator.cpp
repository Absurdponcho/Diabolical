#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <unordered_set>
#include <chrono>
#include <thread>
#include "MetaGenerator.h"


CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData clientData)
{
	VisitorData* visitorData = (VisitorData*)clientData;
	std::ostream* LogInfo = visitorData->LogInfo;

	if (cursor.kind == CXCursor_ClassDecl)
	{
		//*LogInfo << "\n";
	}

	auto cursorKindName = clang_getCString(clang_getCursorKindSpelling(cursor.kind));
	auto cursorEntityName = clang_getCString(clang_getCursorSpelling(cursor));

	auto parentEntityName = clang_getCString(clang_getCursorSpelling(parent));

	ClangMetadata clangMetaData;
	clangMetaData.displayName = clang_getCString(clang_getCursorDisplayName(cursor));
	clangMetaData.entityName = clang_getCString(clang_getCursorSpelling(cursor));
	clangMetaData.cursorKind = clang_getCursorKind(cursor);
	clangMetaData.accessSpecifier = clang_getCXXAccessSpecifier(cursor);
	clangMetaData.typeKind = clang_getCursorType(cursor).kind;
	clangMetaData.typeName = clang_getCString(clang_getTypeKindSpelling(clangMetaData.typeKind));
	clangMetaData.isPtr = clangMetaData.typeKind == CXType_Pointer || clangMetaData.typeKind == CXType_ObjCObjectPointer || clangMetaData.typeKind == CXType_MemberPointer || clangMetaData.typeKind == CXType_BlockPointer;
	clangMetaData.isPOD = clang_isPODType(clang_getCursorType(cursor));
	clangMetaData.returnTypeKind = clang_getCursorResultType(cursor).kind;
	clangMetaData.returnTypeName = clang_getCString(clang_getTypeKindSpelling(clangMetaData.returnTypeKind));
	clangMetaData.arraySize = clang_getArraySize(clang_getCursorType(cursor));
	clangMetaData.parentDisplayName = clang_getCString(clang_getCursorDisplayName(parent));
	clangMetaData.parentEntityName = clang_getCString(clang_getCursorSpelling(parent));
	clangMetaData.parentTypeKind = clang_getCursorType(parent).kind;
	clangMetaData.TypeSize = clang_Type_getSizeOf(clang_getCursorType(cursor));
	clangMetaData.BitOffset = clang_Cursor_getOffsetOfField(cursor);
	clangMetaData.ByteOffset = clang_Cursor_getOffsetOfField(cursor) / 8;

	visitorData->GeneratedClangMetaData.push_back(clangMetaData);

	*LogInfo << "                " << cursorKindName << " : " << cursorEntityName << "\n";	

	return CXChildVisitResult::CXChildVisit_Recurse;
};

std::vector<std::filesystem::path> stdlibpch;

std::string ext(".h");

std::unordered_set<std::string> GetAllHeaders(std::filesystem::path& srcpath)
{
	std::unordered_set<std::string> allheaders;

	for (auto& p : std::filesystem::recursive_directory_iterator(srcpath))
	{
		const std::filesystem::path& path = p.path();
		if (path.extension() != ext) continue;

		const std::filesystem::path& relpath = std::filesystem::relative(path, srcpath);

		allheaders.insert(relpath.string());
	}

	return allheaders;
}

std::vector<std::filesystem::path> GetRelevantFiles(std::filesystem::path& srcpath, std::filesystem::path& pchpath)
{
	std::vector<std::filesystem::path> files;

	for (auto& p : std::filesystem::recursive_directory_iterator(srcpath))
	{
		const std::filesystem::path& path = p.path();
		if (path.extension() != ext) continue;

		const std::filesystem::path& relpath = std::filesystem::relative(path, srcpath);
		std::filesystem::path temppath = pchpath / relpath;
		temppath += ".gen";

		std::string line;
		std::ifstream fin;

		fin.open(path);

		bool bHasAnyMeta = false;

		while (getline(fin, line))
		{
			if (!bHasAnyMeta && line.find("METACLASS") != std::string::npos)
			{
				bHasAnyMeta = true;
				break;
			}
		}

		std::filesystem::create_directories(temppath.parent_path());
		if (bHasAnyMeta)
		{
			files.push_back(std::filesystem::absolute(path));
		}
		fin.close();
	}

	return files;
}

void GetDiagnostics(CXTranslationUnit translationUnit, std::ostream& LogInfo)
{

	int diagnosticnum = clang_getNumDiagnostics(translationUnit);
	LogInfo << "        diagnostic count: " << diagnosticnum << std::endl;
	for (int i = 0; i < diagnosticnum; i++)
	{
		auto diag = clang_getDiagnostic(translationUnit, i);
		auto diagstr = clang_formatDiagnostic(diag, CXDiagnostic_DisplayCategoryName | CXDiagnostic_DisplayCategoryId | CXDiagnostic_DisplayOption | CXDiagnostic_DisplaySourceRanges | CXDiagnostic_DisplayColumn | CXDiagnostic_DisplaySourceLocation);
		const char* cstr = clang_getCString(diagstr);

		//LogInfo << "            diagnostic: " << cstr << '\n';

		clang_disposeString(diagstr);
		clang_disposeDiagnostic(diag);
	}
}

std::string GenerateSpaces(int Amount)
{
	std::string out;
	for (int i = 0; i < Amount; i++)
	{
		out.append(" ");
	}
	return out;
}

void SortClangMetadataHierarchy(std::vector<ClangMetadata>& Metadata)
{
	std::vector<ClangMetadata*> Depth;
	for (ClangMetadata& Meta : Metadata) // sort metadata children into their parents
	{
		while (Depth.size() > 0)
		{
			ClangMetadata* CurrentClass = Depth.at(Depth.size() - 1);
			if (!CurrentClass) break;

			if (Meta.IsChildOf(*CurrentClass))
			{
				CurrentClass->Children.push_back(&Meta);
				Meta.Parent = CurrentClass;
				break;
			}
			else
			{
				Depth.pop_back();
			}
		}

		Depth.push_back(&Meta); 
	}
}

void GenerateMetaHeader(const std::vector<ClangMetadata>& Metadata, const std::string& DestinationFileName, std::filesystem::path& srcpath)
{
	std::filesystem::create_directories(std::filesystem::path(DestinationFileName).parent_path());
	std::ifstream InStream;
	std::string ExistingHeaderContent;
	if (std::filesystem::exists(DestinationFileName))
	{
		InStream.open(DestinationFileName);
		std::stringstream buffer;
		buffer << InStream.rdbuf();
		ExistingHeaderContent = buffer.str();
	}

	std::stringstream NewHeaderContent;
	NewHeaderContent << "#pragma once\n";
	NewHeaderContent << "// Generated metadata for " << srcpath.string() << ".\n";
	NewHeaderContent << "// You probably shouldn't edit this.\n";
	NewHeaderContent << "// However, feel free to read it to understand what is happening.\n";
	NewHeaderContent << "\n";
	NewHeaderContent << "#define CONCAT(a, b) CONCAT_INNER(a, b)\n";
	NewHeaderContent << "#define CONCAT_INNER(a, b) a ## b\n";

	NewHeaderContent << "#ifdef IMPORT_META\n";
	NewHeaderContent << "	#undef IMPORT_META\n";
	NewHeaderContent << "	#ifndef LIBCLANG_META\n";
	NewHeaderContent << "		#define IMPORT_META(x) CONCAT(x, _META());\n";
	NewHeaderContent << "	#else\n";
	NewHeaderContent << "		#define IMPORT_META(x)\n";
	NewHeaderContent << "	#endif\n";
	NewHeaderContent << "#endif\n";

	NewHeaderContent << "#ifndef LIBCLANG_META\n";
	for (const ClangMetadata& Meta : Metadata)
	{
		if (!Meta.IsMetaType()) continue;
		if (Meta.cursorKind == CXCursor_ClassDecl)
		{
			NewHeaderContent << "\n";
			
			// Create the define
			NewHeaderContent << "	#define " << Meta.displayName << "_META()\\\n"; 

			// Some helpful static functions
			NewHeaderContent << "		public:\\\n";
			NewHeaderContent << "		__forceinline static const char* GetClassName() { return \"" << Meta.displayName << "\"; }\\\n";
			NewHeaderContent << "		__forceinline static const size_t GetClassSize() { return " << Meta.TypeSize << "; }\\\n";
			NewHeaderContent << "		\\\n";

			// Function to get property from its name
			NewHeaderContent << "		template <typename T>\\\n";
			NewHeaderContent << "		__forceinline constexpr T* GetPropertyPtr(const char* const PropertyName) const\\\n";
			NewHeaderContent << "		{\\\n";
			for (const ClangMetadata* ChildMeta : Meta.Children)
			{
				if (!ChildMeta->IsMetaType()) continue;
				NewHeaderContent << "			if (strcmp(PropertyName, \"" << ChildMeta->displayName << "\") == 0)\\\n";
				NewHeaderContent << "			{\\\n";
				NewHeaderContent << "				if (typeid(" << ChildMeta->displayName << ") == typeid(T))\\\n";
				NewHeaderContent << "					{ return (T*)&" << ChildMeta->displayName << "; }\\\n";
				NewHeaderContent << "				else\\\n";
				NewHeaderContent << "					{ return nullptr; }\\\n";
				NewHeaderContent << "			}\\\n";

			}
			NewHeaderContent << "			return nullptr;\\\n";
			NewHeaderContent << "		}\\\n";

			// Function to get property from offset
			NewHeaderContent << "		template <typename T>\\\n";
			NewHeaderContent << "		__forceinline constexpr T* GetPropertyPtrFromOffset(const size_t ByteOffset) const\\\n";
			NewHeaderContent << "		{\\\n";
			for (const ClangMetadata* ChildMeta : Meta.Children)
			{
				if (!ChildMeta->IsMetaType()) continue;
				NewHeaderContent << "			if (" << ChildMeta->ByteOffset << " == ByteOffset)\\\n";
				NewHeaderContent << "			{\\\n";
				NewHeaderContent << "				if (typeid(" << ChildMeta->displayName << ") == typeid(T))\\\n";
				NewHeaderContent << "					{ return (T*)&" << ChildMeta->displayName << "; }\\\n";
				NewHeaderContent << "				else\\\n";
				NewHeaderContent << "					{ return nullptr; }\\\n";
				NewHeaderContent << "			}\\\n";
			}
			NewHeaderContent << "			return nullptr;\\\n";
			NewHeaderContent << "		}\\\n";
		}
		else
		{
			// lets add useful functions for properties and methods
		}

		if (Meta.accessSpecifier == CX_CXXPrivate) NewHeaderContent << "		private:\\\n";
		if (Meta.accessSpecifier == CX_CXXProtected) NewHeaderContent << "		protected:\\\n";
	}
	NewHeaderContent << "\n";
	NewHeaderContent << "#endif\n";

	std::string NewHeaderString = NewHeaderContent.str();

	if (!NewHeaderString._Equal(ExistingHeaderContent)) 
	{
		// only save the file if it is different to the original or else 
		// libclang will think its new anyway and ignore the pch
		std::ofstream Stream;
		Stream.open(DestinationFileName);
		Stream << NewHeaderString;
		Stream.close();
	}
}

void ParseHeader(std::filesystem::path& HeaderPath, std::filesystem::path& srcpath, std::filesystem::path& pchpath, std::filesystem::path& metapath, std::ostream& LogInfo, long long& millisTotal)
{
	LogInfo << "    GENERATING: " + HeaderPath.string() << '\n';
	CXIndex Index = nullptr;

	std::string includedir = ("-I" + srcpath.string());

	const std::filesystem::path& relpath = std::filesystem::relative(HeaderPath, srcpath);
	std::filesystem::path temppath = pchpath / relpath;
	std::filesystem::path metatemppath = metapath / relpath;

	std::string metaincludedir = ("-I" + metapath.string());

	std::string pchfile = temppath.string() + ".pch";

	auto start = std::chrono::system_clock::now();

	CXTranslationUnit translationUnit = nullptr;

	if (std::filesystem::exists(pchfile))
	{
		Index = clang_createIndex(0, 1); 
		LogInfo << "        Using PCH: " << pchfile << '\n';
		CXErrorCode Error = clang_createTranslationUnit2(Index, pchfile.c_str(), &translationUnit);
		if (Error != 0) // if this fails then the source file is likely newer. so this pch is now invalid
		{
			LogInfo << "        Using PCH Failed. This can indicate the source code has been modified.\n        CXErrorCode: " << (int)Error << std::endl;
			GetDiagnostics(translationUnit, LogInfo);
			clang_disposeTranslationUnit(translationUnit);
			translationUnit = nullptr;
			clang_disposeIndex(Index);
			Index = nullptr;
		}
	}
	
	if (translationUnit == nullptr) // if using the pch failed we can read the source and generate a pch
	{
		std::vector<const char*> args{ "--language=c++", includedir.c_str(), metaincludedir.c_str(), "-std=c++17", "-D", "LIBCLANG_META"};
		Index = clang_createIndex(0, 0);
		args.push_back("-Xclang");
		args.push_back("-emit-pch"); 
		args.push_back("-o");
		args.push_back(pchfile.c_str());
		LogInfo << "        Generating PCH: " << pchfile << '\n';
		CXErrorCode Error = clang_parseTranslationUnit2(Index, HeaderPath.string().c_str(), args.data(), (int)args.size(), nullptr, 0, CXTranslationUnit_Incomplete | CXTranslationUnit_ForSerialization | CXTranslationUnit_SkipFunctionBodies | CXTranslationUnit_KeepGoing | CXTranslationUnit_PrecompiledPreamble, &translationUnit);
		if (Error != 0)
		{
			LogInfo << "        Generating PCH Failed. CXErrorCode: " << (int)Error << std::endl;
			GetDiagnostics(translationUnit, LogInfo);
			clang_disposeTranslationUnit(translationUnit);
			translationUnit = nullptr;
			clang_disposeIndex(Index);
			Index = nullptr;
		}
		else
		{
			clang_saveTranslationUnit(translationUnit, pchfile.c_str(), 0);
		}
	}

	std::stringstream VisitorLogInfo = std::stringstream();
	VisitorData visitorData = { &VisitorLogInfo };

	if (translationUnit)
	{
		auto cursor = clang_getTranslationUnitCursor(translationUnit);
		clang_visitChildren(cursor, visitor, &visitorData);
		clang_disposeTranslationUnit(translationUnit);
	}

	if (Index)
	{
		clang_disposeIndex(Index);
	}

	auto end = std::chrono::system_clock::now();

	auto difference = end - start;
	auto millis = duration_cast<std::chrono::milliseconds>(difference).count();

	millisTotal += millis;

	if (true)
	{
		LogInfo << VisitorLogInfo.str();
	}

	SortClangMetadataHierarchy(visitorData.GeneratedClangMetaData);

	std::string metatemppathstr = (metapath / metatemppath.filename()).string();

	GenerateMetaHeader(visitorData.GeneratedClangMetaData, metatemppathstr.substr(0, metatemppathstr.find_last_of('.')) + ".meta.h", srcpath);

	LogInfo << "        Complete in " << millis << "ms\n";
}

void RunThread(ThreadData& Data)
{
	std::stringstream result;
	for (auto& path : Data.files)
	{
		ParseHeader(path, Data.srcpath, Data.pchpath, Data.metapath, result, Data.millisTotal);
	}
	Data.result = result.str();
}

void RunProject(const std::filesystem::path& root)
{
	std::cout << "Project:" << root.string() << "\n";

	std::filesystem::path pchpath = root / "meta_gen\\pch\\";
	std::filesystem::path metapath = root / "meta_gen\\codegen\\";
	std::filesystem::path srcpath = root / "src\\"; 


	std::unordered_set<std::string> allheaders = GetAllHeaders(srcpath);
	std::vector<std::filesystem::path> files = GetRelevantFiles(srcpath, pchpath);

	if (files.size() == 0)
	{
		std::cout << "    Invalid Arguments, no files specified\n";
		return;
	}

	int ThreadCount = std::thread::hardware_concurrency() / 2;
	int FilesPerThread = (int)std::ceil((float)files.size() / (float)ThreadCount);

	std::cout << "    Maximum threads: " << ThreadCount << std::endl;
	std::cout << "    Total Files: " << files.size() << std::endl;

	std::vector<std::thread> SpawnedThreads;
	std::vector<ThreadData> Data;

	for (int i = 0; i < ThreadCount; i++) // generate thread data
	{
		Data.push_back({ std::vector<std::filesystem::path>(), srcpath, pchpath, metapath, std::string()});
		Data.at(i).files.reserve(FilesPerThread);
	}

	int DataIndex = 0;
	for (auto file : files) // add files to thread data
	{
		Data.at(DataIndex).files.push_back(file);
		DataIndex++;
		if (DataIndex >= Data.size())
		{
			DataIndex = 0;
		}
	}

	for (int i = 0; i < ThreadCount; i++) // spawn the threads, pass thread data and function
	{
		if (Data.at(i).files.size() > 0)
		{
			SpawnedThreads.push_back(std::thread(RunThread, std::ref(Data.at(i))));
		}
	}

	std::cout << "    Utilizing " << SpawnedThreads.size() << " threads." << std::endl;

	for (std::thread& Thread : SpawnedThreads) // wait for threads to finish
	{
		Thread.join();
	}

	long long totalMillis = 0;
	for (ThreadData& Result : Data) // display result from all threads
	{
		totalMillis += Result.millisTotal;
		std::cout << Result.result;
	}

	std::cout << "    Total time spent all threads combined: " << totalMillis << "ms." << std::endl;
}

int main(int argc, const char* argv[])
{
	std::cout << "Starting Code Generation\n";

	std::vector<std::filesystem::path> projects;

	std::string root("../");
	std::string ext(".cfg");

	for (auto& p : std::filesystem::recursive_directory_iterator(root)) 
	{
		const std::filesystem::path& path = p.path();
		if (path.extension() != ext) continue;
		if (path.filename().string() != "MetaConfig.cfg") continue;

		projects.push_back(std::filesystem::absolute(path.parent_path()));
	}

	for (auto& projectroot : projects)
	{
		auto start = std::chrono::system_clock::now();

		RunProject(projectroot);

		auto end = std::chrono::system_clock::now();

		auto millis = duration_cast<std::chrono::milliseconds>(end - start).count();

		std::cout << "    Completed Project " << std::filesystem::relative(projectroot, "../") << " in " << millis << "ms\n";
	}
}

