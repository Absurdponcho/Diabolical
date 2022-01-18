#pragma once

#include <string>
#include <vector>
// programming for windows first, since I cant test on other platforms rn
// remindme put windows macro in
#include <Windows.h>
#include <filesystem>
#include <fstream>
namespace diabolical
{
	bool DirCreate(const std::string& dir);
	bool DirCopy(const std::string& src, const std::string& dest);
	bool PathExists(const std::string& path);
	bool PathDelete(const std::string& path);

	bool FileCreate(const std::string& file);
	bool FileCopy(const std::string& src, const std::string& dest);
	std::vector <const std::filesystem::path> FilesIn(const std::string& dir, bool recursive = false);

}