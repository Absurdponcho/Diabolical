#include "Filesystem.h"
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
bool DFileSystem::DirCreate(const DString& dir)
{
	if (PathExists(dir))
		return false;

	return std::filesystem::create_directories(*dir);
}

bool DFileSystem::DirCopy(const DString& src, const DString& dest)
{
	if (PathExists(src)) {
		std::filesystem::copy(*src, *dest);
		return true;
	}
	return false;
}

bool DFileSystem::PathExists(const DString& path)
{
	return std::filesystem::exists(*path);
}

bool DFileSystem::FileCreate(const DString& file, std::ofstream& stream, bool bOverwrite)
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

bool DFileSystem::FileCopy(const DString& src, const DString& dest)
{
	if (PathExists(src)) {
		return std::filesystem::copy_file(*src, *dest);
	}
	return false;
}

bool DFileSystem::PathDelete(const DString& path)
{
	if (PathExists(path)) {
		return std::filesystem::remove(*path);
	}
	return false;
}

DVector <DString> DFileSystem::FilesIn(const DString& dir, bool recursive)
{
	DVector <DString> filesFound;
	if (!recursive)
	{
		for (const auto& entry : std::filesystem::directory_iterator(*dir))
			filesFound.PushBack(entry.path());
	}
	else
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(*dir))
			filesFound.PushBack(entry.path());
	}
	return filesFound;
}

uint64_t DFileSystem::FileSize(const DString& Path)
{
	if (!PathExists(Path)) return 0;

	return (uint64_t) std::filesystem::file_size(*Path);
}


DString DFileSystem::NormalizeFilePath(DString& FilePath, bool bAbsolute)
{
	std::error_code Error;
	std::filesystem::path NewPath = std::filesystem::relative(std::filesystem::path(*FilePath), std::filesystem::current_path(), Error);
	
	if (Error)
	{
		return Error.message();
	}

	if (bAbsolute)
	{
		return DString::Format("%s\\%s", *CurrentDirectory(), NewPath.string().c_str()).CapitalizePath();
	}

	return DString(NewPath.string()).CapitalizePath();
}

DString DFileSystem::CurrentDirectory()
{
	return std::filesystem::current_path().string() + '\\';
}
