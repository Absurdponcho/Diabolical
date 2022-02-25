#pragma once
#include "Types/DString.h"

namespace DFileSystem
{
	bool DirCreate(const DString& dir);
	bool DirCopy(const DString& src, const DString& dest);
	bool PathExists(const DString& path);
	bool PathDelete(const DString& path);

	bool FileCreate(const DString& file, std::ofstream& stream, bool bOverwrite = true);
	bool FileCopy(const DString& src, const DString& dest);
	uint64_t FileSize(const DString& Path);
	DVector <DString> FilesIn(const DString& dir, bool recursive = false);
	DString NormalizeFilePath(DString& FilePath, bool bAbsolute = false);
	DString CurrentDirectory();

}