#include "Filesystem.h"
bool diabolical::DirCreate(const std::string& dir)
{
	if (PathExists(dir))
		return false;

	return std::filesystem::create_directories(dir);
}

bool diabolical::DirCopy(const std::string& src, const std::string& dest)
{
	if (PathExists(src)) {
		std::filesystem::copy(src, dest);
		return true;
	}
	return false;
}

bool diabolical::PathExists(const std::string& path)
{
	return std::filesystem::exists(path);
}

bool diabolical::FileCreate(const std::string& file)
{
	if (PathExists(file))
		return false;
	std::ofstream ofs(file);
	ofs.close(); // should we return an fstream?
	return true;
}

bool diabolical::FileCopy(const std::string& src, const std::string& dest)
{
	if (PathExists(src)) {
		return std::filesystem::copy_file(src, dest);
	}
	return false;
}

bool diabolical::PathDelete(const std::string& path)
{
	if (PathExists(path)) {
		return std::filesystem::remove(path);
	}
	return false;
}

std::vector <const std::filesystem::path> diabolical::FilesIn(const std::string& dir, bool recursive)
{
	std::vector <const std::filesystem::path> filesFound;
	if (!recursive)
	{
		for (const auto& entry : std::filesystem::directory_iterator(dir))
			filesFound.push_back(entry.path());
	}
	else
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(dir))
			filesFound.push_back(entry.path());
	}
	return filesFound;
}
