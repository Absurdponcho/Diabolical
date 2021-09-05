#pragma once
#include "GameAsset.h"
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
}; // sizeof = 12

struct FileDescriptor
{
	char Name[64];
	int64_t Offset;
	int64_t Size;
	int64_t SizeUncompressed;
	bool bZlibCompressed;
	// char pad[3];
}; // sizeof = 92

struct Archive
{
	ArchiveHeader ArcHeader;
	FileDescriptor ArcFiles[1];
};

bool GenerateArchive(std::filesystem::path Path);

class ArchiveAsset : public GameAsset {
public:
	static ArchiveAsset* TryLoad(std::filesystem::path Path);
	const uint8_t* GetAssetData() override { return nullptr; }; // do not use
    const uint8_t* GetAssetData(int64_t index);
	const uint8_t* GetAssetData(std::string AssetPath);
	const size_t GetAssetSize(int64_t index);
	const size_t GetAssetSize(std::string AssetPath);
protected:
	Archive* Arc;
	std::ifstream FileStream;
	std::vector<FileDescriptor> Files; 
};