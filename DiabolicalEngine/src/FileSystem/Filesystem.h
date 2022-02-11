#pragma once

#include "../Types/DString.h"
#include <string>
#include <vector>
// programming for windows first, since I cant test on other platforms rn
// remindme put windows macro in
#include <Windows.h>
#include <filesystem>
#include <fstream>
namespace diabolical
{
	bool DirCreate(const DString& dir);
	bool DirCopy(const DString& src, const DString& dest);
	bool PathExists(const DString& path);
	bool PathDelete(const DString& path);

	bool FileCreate(const DString& file, std::ofstream& stream, bool bOverwrite = true);
	bool FileCopy(const DString& src, const DString& dest);
	DVector <std::filesystem::path> FilesIn(const DString& dir, bool recursive = false);

}