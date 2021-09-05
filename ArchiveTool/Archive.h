#include <Windows.h>
#include <fstream>
#include <filesystem>
#include <cstdint>
#include <vector>
#include <zlib.h>

const int64_t ARCHIVE_VALID_MAGIC = 0xD1AB011CA1;

const int64_t ARCHIVE_VER_ID = 1;

struct ArchiveHeader {
	int64_t Magic;
	int64_t VersionID;
	int64_t NumberOfFiles;
};

struct FileDescriptor
{
	char Name[64];
	int64_t Offset;
	int64_t Size;
	int64_t SizeUncompressed;
	bool bZlibCompressed;
};

struct Archive
{
	ArchiveHeader ArcHeader;
	FileDescriptor ArcFiles[1];
};

bool GenerateArchive(std::filesystem::path Path);