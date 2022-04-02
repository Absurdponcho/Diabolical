#include <iostream>
#include <fstream>
#include <vector>
#include "clang-c/Index.h"
#include <filesystem>
#include <unordered_set>
#include <chrono>
#include <thread>

struct VisitorData
{
	std::ostream* LogInfo;
	bool bJustVisitedClassDecl = false;
	bool bWithinClassDecl = false;
	std::string ParentName = std::string();
	bool bWithinInvalidClass = false;
};

CXChildVisitResult Visitor2(CXCursor cursor, CXCursor parent, CXClientData clientData)
{
	VisitorData* visitorData = (VisitorData*)clientData;
	std::ostream* LogInfo = visitorData->LogInfo;

	auto cursorKindName = clang_getCString(clang_getCursorKindSpelling(cursor.kind));
	auto cursorEntityName = clang_getCString(clang_getCursorSpelling(cursor));

	auto parentEntityName = clang_getCString(clang_getCursorSpelling(parent));

	if (visitorData->bWithinClassDecl) // we are inside of a valid class declaration, so let's keep going deeper
	{
		if (cursor.kind == CXCursor_ClassDecl) // this is a new class declaration
		{
			if (std::string(visitorData->ParentName)._Equal(parentEntityName)) // This is a class declaration within our current class. For simplicity, generate meta for this class.
			{
				return CXChildVisitResult::CXChildVisit_Recurse;
			}
			else // We have left the current class. reset things and move on to the next "if" block
			{
				visitorData->bJustVisitedClassDecl = false;
				visitorData->bWithinClassDecl = false;
				visitorData->ParentName = std::string();
			}
		}
		else // include everything inside of this class
		{
			return CXChildVisitResult::CXChildVisit_Recurse;
		}
	}


	if (!visitorData->bWithinClassDecl) // we aren't inside of a valid class declaration so we can look for it and check it's first child for the "meta" annotation
	{
		if (cursor.kind == CXCursor_ClassDecl)
		{
			visitorData->bJustVisitedClassDecl = true;
			visitorData->ParentName = std::string(cursorEntityName);
			return CXChildVisitResult::CXChildVisit_Recurse;
		}
		else if (cursor.kind == CXCursor_AnnotateAttr && visitorData->bJustVisitedClassDecl)
		{
			visitorData->bJustVisitedClassDecl = false;
			visitorData->ParentName = std::string();
			if (!std::string(cursorEntityName)._Equal("meta"))
			{
				return CXChildVisitResult::CXChildVisit_Continue;
			}
			else
			{
				visitorData->bJustVisitedClassDecl = true;
				visitorData->ParentName = std::string(cursorEntityName);
				visitorData->bWithinClassDecl = true;
				*LogInfo << "            Found Meta Class \"" << parentEntityName << "\"\n";
				return CXChildVisitResult::CXChildVisit_Recurse;

			}
		}
		else
		{
			if (!visitorData->bWithinInvalidClass)
			{
				visitorData->bWithinInvalidClass = true;
				*LogInfo << "            Found Non-Meta Class \"" << parentEntityName << "\"\n";
			}
			visitorData->ParentName = std::string();
			visitorData->bJustVisitedClassDecl = false;
		}

		return CXChildVisitResult::CXChildVisit_Continue;
	}

	return CXChildVisitResult::CXChildVisit_Continue;
}

CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData clientData)
{
	VisitorData* visitorData = (VisitorData*)clientData;
	std::ostream* LogInfo = visitorData->LogInfo;

	if (cursor.kind == CXCursor_ClassDecl)
	{
		*LogInfo << "\n";
	}


	auto cursorKindName = clang_getCString(clang_getCursorKindSpelling(cursor.kind));
	auto cursorEntityName = clang_getCString(clang_getCursorSpelling(cursor));

	auto parentEntityName = clang_getCString(clang_getCursorSpelling(parent));

	CXChildVisitResult Result = Visitor2(cursor, parent, clientData);

	if (visitorData->bWithinClassDecl)
	{
		visitorData->bWithinInvalidClass = false;
		*LogInfo << "                " << cursorKindName << " : " << cursorEntityName << "\n";
	}

	return Result;
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

std::vector<std::filesystem::path> GetRelevantFiles(std::filesystem::path& srcpath, std::filesystem::path& metapath)
{
	std::vector<std::filesystem::path> files;

	for (auto& p : std::filesystem::recursive_directory_iterator(srcpath))
	{
		const std::filesystem::path& path = p.path();
		if (path.extension() != ext) continue;

		const std::filesystem::path& relpath = std::filesystem::relative(path, srcpath);
		std::filesystem::path temppath = metapath / relpath;
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

		LogInfo << "            diagnostic: " << cstr << '\n';

		clang_disposeString(diagstr);
		clang_disposeDiagnostic(diag);
	}
}

void ParseHeader(std::filesystem::path& HeaderPath, std::filesystem::path& srcpath, std::filesystem::path& metapath, std::ostream& LogInfo, long long& millisTotal)
{
	LogInfo << "    GENERATING: " + HeaderPath.string() << '\n';
	CXIndex Index = nullptr;

	std::string includedir = ("-I" + srcpath.string());

	const std::filesystem::path& relpath = std::filesystem::relative(HeaderPath, srcpath);
	std::filesystem::path temppath = metapath / relpath;

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
		std::vector<const char*> args{ "--language=c++", includedir.c_str(), "-std=c++17" };
		Index = clang_createIndex(0, 1);
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

	if (translationUnit)
	{
		VisitorData visitorData = { &LogInfo };

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

	LogInfo << "        Complete in " << millis << "ms\n";
}

struct ThreadData
{
	std::vector<std::filesystem::path> files;
	std::filesystem::path srcpath;
	std::filesystem::path metapath;
	std::string result;
	long long millisTotal;
};

void RunThread(ThreadData& Data)
{
	std::stringstream result;
	for (auto& path : Data.files)
	{
		ParseHeader(path, Data.srcpath, Data.metapath, result, Data.millisTotal);
	}
	Data.result = result.str();
}

void RunProject(const std::filesystem::path& root)
{
	std::cout << "Project:" << root.string() << "\n";

	std::filesystem::path metapath = root / "meta_gen/";
	std::filesystem::path srcpath = root / "src/"; 


	std::unordered_set<std::string> allheaders = GetAllHeaders(srcpath);
	std::vector<std::filesystem::path> files = GetRelevantFiles(srcpath, metapath);

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
		Data.push_back({ std::vector<std::filesystem::path>(), srcpath, metapath, std::string()});
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

