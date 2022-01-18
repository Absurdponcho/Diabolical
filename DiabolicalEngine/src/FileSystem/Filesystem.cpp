#include "Filesystem.h"
bool diabolical::DirCreate(const DString& dir)
{
	if (PathExists(dir))
		return false;

	return std::filesystem::create_directories(*dir);
}

bool diabolical::DirCopy(const DString& src, const DString& dest)
{
	if (PathExists(src)) {
		std::filesystem::copy(*src, *dest);
		return true;
	}
	return false;
}

bool diabolical::PathExists(const DString& path)
{
	return std::filesystem::exists(*path);
}

bool diabolical::FileCreate(const DString& file, std::ofstream& stream, bool bOverwrite)
{
	if (!bOverwrite && PathExists(file))
		return false;

	int LastSlash = file.FindLast("/");

	if (LastSlash == -1)
		return false;

	DString Path, File;
	file.Split(LastSlash, Path, File);

	std::filesystem::create_directories(*Path);

	stream = std::ofstream(*file);
	return true;
}

bool diabolical::FileCopy(const DString& src, const DString& dest)
{
	if (PathExists(src)) {
		return std::filesystem::copy_file(*src, *dest);
	}
	return false;
}

bool diabolical::PathDelete(const DString& path)
{
	if (PathExists(path)) {
		return std::filesystem::remove(*path);
	}
	return false;
}

std::vector <std::filesystem::path> diabolical::FilesIn(const DString& dir, bool recursive)
{
	std::vector <std::filesystem::path> filesFound;
	if (!recursive)
	{
		for (const auto& entry : std::filesystem::directory_iterator(*dir))
			filesFound.push_back(entry.path());
	}
	else
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(*dir))
			filesFound.push_back(entry.path());
	}
	return filesFound;
}
