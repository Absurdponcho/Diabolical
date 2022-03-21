#include <iostream>
#include <fstream>
#include <vector>
#include "clang-c/Index.h"
#include <filesystem>
#include <unordered_set>

CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData clientData)
{
	auto cursorKindName = clang_getCString(clang_getCursorKindSpelling(cursor.kind));
	auto cursorEntityName = clang_getCString(clang_getCursorSpelling(cursor));
	std::cout << "        " << cursorKindName << " : " << cursorEntityName << "\n";
	
	return CXChildVisitResult::CXChildVisit_Recurse;
};

void RunProject(const std::filesystem::path& root)
{
	std::cout << "Project:" << root.string() << "\n";

	std::filesystem::path metapath = root / "meta_gen/";
	std::filesystem::path srcpath = root / "src/";

	std::vector<std::filesystem::path> files;

	std::string ext(".h");

	std::filesystem::remove_all(metapath);

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

		std::string out;
		out.reserve(std::filesystem::file_size(path));
		bool bHasAnyMeta = false;

		while (getline(fin, line)) {
			if (line.find("#include") != std::string::npos)
			{
				// possibly include our own headers? idk if this is needed


				/*std::string includefilename;
				bool bInQuotes = false;
				for (char c : line)
				{
					if (!bInQuotes)
					{
						if (c == '"')
						{
							bInQuotes = true;
						}
					}
					else
					{
						if (c == '"') break;

						if (c == '/') c = '\\';

						includefilename += c;
					}
				}
				if (includefilename.length() != 0 && allheaders.find(includefilename) != allheaders.end())
				{
					out += line + "\n";
				}*/
			}
			else
			{
				out += line + "\n";
			}
			if (!bHasAnyMeta && line.find("GENERATE_META") != std::string::npos)
			{
				if (line.find("#define GENERATE_META") == std::string::npos)
				{
					bHasAnyMeta = true;
				}
			}
		}

		if (bHasAnyMeta)
		{
			std::ofstream temp;
			std::filesystem::create_directories(temppath.parent_path());
			temp.open(temppath);
			if (!temp) { // file couldn't be opened 
				std::cerr << "Error: temp file could not be opened (" << temppath.string() << ")" << std::endl;
				break;
			}
			temp << out;
			temp.close();
			files.push_back(std::filesystem::absolute(temppath));

		}
		fin.close();
	}

	if (files.size() == 0)
	{
		std::cout << "    Invalid Arguments, no files specified\n";
		return;
	}

	for (auto& path : files)
	{
		std::cout << "    GENERATING: " + path.string() << '\n';

		const char* args[] = { "--language=c++" };
		auto index = clang_createIndex(0, 0);
		auto translationUnit = clang_parseTranslationUnit(index, path.string().c_str(), args, 1, nullptr, 0, CXTranslationUnit_Incomplete | CXTranslationUnit_SkipFunctionBodies);
		if (translationUnit == nullptr)
		{
			std::cout << "    FAILED\n";
		}
		auto cursor = clang_getTranslationUnitCursor(translationUnit);
		clang_visitChildren(cursor, visitor, nullptr);
		clang_disposeTranslationUnit(translationUnit);
		clang_disposeIndex(index);

	}
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
		RunProject(projectroot);
	}
}

