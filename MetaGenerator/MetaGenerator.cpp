#include <iostream>
#include <fstream>
#include <vector>
#include "clang-c/Index.h"
#include <filesystem>
#include <unordered_set>
#include <chrono>

CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData clientData)
{
	auto cursorKindName = clang_getCString(clang_getCursorKindSpelling(cursor.kind));
	auto cursorEntityName = clang_getCString(clang_getCursorSpelling(cursor));
	//std::cout << "            " << cursorKindName << " : " << cursorEntityName << "\n";
	
	return CXChildVisitResult::CXChildVisit_Continue;
};

std::vector<std::filesystem::path> stdlibpch;

void CacheSTDLib()
{

}

void RunProject(const std::filesystem::path& root)
{
	std::cout << "Project:" << root.string() << "\n";

	std::filesystem::path metapath = root / "meta_gen/";
	std::filesystem::path srcpath = root / "src/"; 

	std::vector<std::filesystem::path> files;

	std::string ext(".h");

	std::unordered_set<std::string> allheaders;

	for (auto& p : std::filesystem::recursive_directory_iterator(srcpath))
	{
		const std::filesystem::path& path = p.path();
		if (path.extension() != ext) continue;

		const std::filesystem::path& relpath = std::filesystem::relative(path, srcpath);

		allheaders.insert(relpath.string());
	}

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

		//std::string out;
		//out.reserve(std::filesystem::file_size(path));
		bool bHasAnyMeta = false;

		while (getline(fin, line)) { 
			//if (line.find("#include") != std::string::npos)
			//{
			//	// possibly include our own headers? idk if this is needed
			//	std::string includefilename;
			//	bool bInQuotes = false;
			//	for (char c : line)
			//	{
			//		if (!bInQuotes)
			//		{ 
			//			if (c == '"')
			//			{
			//				bInQuotes = true;
			//			}
			//		}
			//		else
			//		{
			//			if (c == '"') break;

			//			if (c == '/') c = '\\'; 

			//			includefilename += c;
			//		}
			//	}
			//	
			//  includefilename = std::filesystem::relative(std::filesystem::path(path.parent_path() / includefilename), srcpath).string(); // make all includes relative to srcpath
			//	
			//	if (includefilename.length() != 0 && allheaders.find(includefilename) != allheaders.end())
			//	{
			//		out += "#include \"" + includefilename + ".gen\"\n";
			//	}
			//	else
			//	{
			//		out += line + "\n";
			//	}
			//}
			//else
			//{
			//	out += line + "\n"; 
			//}

			if (!bHasAnyMeta && line.find("GENERATE_META") != std::string::npos)
			{
				if (line.find("#define GENERATE_META") == std::string::npos)
				{
					bHasAnyMeta = true;
					break;
				}
			}
		}

		
		//std::ofstream temp;
		std::filesystem::create_directories(temppath.parent_path());
		//temp.open(temppath);
		//if (!temp) { // file couldn't be opened 
		//	std::cerr << "Error: temp file could not be opened (" << temppath.string() << ")" << std::endl;
		//	break;
		//}
		//temp << out;
		//temp.close();
		if (bHasAnyMeta)
		{
			files.push_back(std::filesystem::absolute(path));
		}
		fin.close();
	}

	if (files.size() == 0)
	{
		std::cout << "    Invalid Arguments, no files specified\n";
		return;
	}

	std::string includedir = ("-I" + srcpath.string());
	std::cout << "    includedir: " + includedir << '\n';
	auto index = clang_createIndex(0, 0);

	for (auto& path : files)
	{
		std::cout << "    GENERATING: " + path.string() << '\n'; 
		
		const std::filesystem::path& relpath = std::filesystem::relative(path, srcpath);
		std::filesystem::path temppath = metapath / relpath;

		std::string pchfile = temppath.string() + ".pch";

		std::vector<const char*> args{ "--language=c++", includedir.c_str(), "-std=c++17" };		


		auto start = std::chrono::system_clock::now();

		CXTranslationUnit translationUnit = nullptr;
		
		bool bShouldSave = false;

		if (std::filesystem::exists(pchfile))
		{
			args.push_back("-Xclang");
			args.push_back("-include-pch");
			args.push_back(pchfile.c_str());
			std::cout << "        Using PCH: " << pchfile << '\n';
			translationUnit = clang_createTranslationUnit(index, pchfile.c_str());

		}
		else
		{
			args.push_back("-Xclang");
			args.push_back("-emit-pch");
			args.push_back("-o");
			args.push_back(pchfile.c_str());
			std::cout << "        Generating PCH: " << pchfile << '\n';
			translationUnit = clang_parseTranslationUnit(index, path.string().c_str(), args.data(), (int)args.size(), nullptr, 0, CXTranslationUnit_Incomplete | CXTranslationUnit_ForSerialization | CXTranslationUnit_SkipFunctionBodies | CXTranslationUnit_KeepGoing | CXTranslationUnit_PrecompiledPreamble);
			bShouldSave = true;
		}

		if (translationUnit == nullptr)
		{
			std::cout << "    FAILED\n"; 
		}
		
		int diagnosticnum = clang_getNumDiagnostics(translationUnit);
		for (int i = 0; i < diagnosticnum; i++)
		{
			auto diag = clang_getDiagnostic(translationUnit, i);
			auto diagstr = clang_formatDiagnostic(diag, CXDiagnostic_DisplayCategoryName | CXDiagnostic_DisplayCategoryId | CXDiagnostic_DisplayOption | CXDiagnostic_DisplaySourceRanges | CXDiagnostic_DisplayColumn | CXDiagnostic_DisplaySourceLocation);
			const char* cstr = clang_getCString(diagstr);

			std::cout << "            diagnostic: " << cstr << '\n';

			clang_disposeString(diagstr);
			clang_disposeDiagnostic(diag);
		}

		if (bShouldSave)
		{
			clang_saveTranslationUnit(translationUnit, pchfile.c_str(), 0);
		}

		auto cursor = clang_getTranslationUnitCursor(translationUnit);
		clang_visitChildren(cursor, visitor, nullptr);
		clang_disposeTranslationUnit(translationUnit);
		
		auto end = std::chrono::system_clock::now();
		 
		auto difference = end - start;
		auto millis = duration_cast<std::chrono::milliseconds>(difference).count();

		std::cout << "        Complete in " << millis << "ms\n";
	}
	clang_disposeIndex(index);
}

int main(int argc, const char* argv[])
{
	std::cout << "Starting Code Generation\n";

	CacheSTDLib();

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

