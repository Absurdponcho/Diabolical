#include "ArchiveAsset.h"
#include "../Logging/Logging.h"

ArchiveAsset* ArchiveAsset::TryLoad(std::filesystem::path Path)
{
	if (!std::filesystem::exists(Path))
	{
		return nullptr;
	}
	ArchiveAsset* LoadedArchive = new ArchiveAsset();
	ArchiveHeader ArcHeader = ArchiveHeader();

	LoadedArchive->FileStream = std::ifstream(Path.string(), std::ios::binary | std::ios::in);
	LoadedArchive->FileStream.read((char*)&ArcHeader, sizeof(ArchiveHeader));
	if (ArcHeader.Magic != ARCHIVE_VALID_MAGIC ||
		ArcHeader.VersionID != ARCHIVE_VER_ID ||
		ArcHeader.NumberOfFiles == 0)
	{
		LoadedArchive->FileStream.close();
		return nullptr;
	}
	LoadedArchive->FileStream.seekg(0);
	size_t ArchiveIndexSize = sizeof(ArchiveHeader) + (sizeof(FileDescriptor) * ArcHeader.NumberOfFiles);
	if (Archive* Arc = (Archive*)malloc(ArchiveIndexSize)) {

		LoadedArchive->FileStream.read((char*)Arc, ArchiveIndexSize);
		// Validate Archives
		for (int64_t i = 0; i < Arc->ArcHeader.NumberOfFiles; i++) {
			if (!Arc->ArcFiles[i].Offset || !Arc->ArcFiles[i].Size) {
				//failed
				free(Arc);
				LoadedArchive->FileStream.close();
				LoadedArchive->Files.clear();
				return nullptr;
			}
			LoadedArchive->NameIndexMap.emplace(std::string(Arc->ArcFiles[i].Name), i);
			LoadedArchive->Files.push_back(Arc->ArcFiles[i]);
		}
	}
	LoadedGameAssets.push_back(LoadedArchive);
	return LoadedArchive;
}

const uint8_t* ArchiveAsset::GetAssetData(int64_t index)
{
	auto File = Files[index];
	uint8_t* Data = (uint8_t*)malloc(File.Size);
	FileStream.seekg(File.Offset);
	FileStream.read((char*)Data, File.Size);
	if (File.bZlibCompressed)
	{
		uint8_t* DataInflated = (uint8_t*)malloc(File.SizeUncompressed);
		z_stream InflateStream;
		InflateStream.zalloc = Z_NULL;
		InflateStream.zfree = Z_NULL;
		InflateStream.opaque = Z_NULL;
		InflateStream.avail_in = (uInt)File.Size;
		InflateStream.next_in = (Bytef*)Data;
		InflateStream.avail_out = (uInt)File.SizeUncompressed;
		InflateStream.next_out = (Bytef*)DataInflated;
		inflateInit(&InflateStream);
		if (inflate(&InflateStream, Z_NO_FLUSH) != Z_STREAM_END)
		{
			inflateEnd(&InflateStream);
			free(Data);
			free(DataInflated);
			return nullptr;
		};
		inflateEnd(&InflateStream); 
		free(Data);
		return DataInflated;
	}
	return Data;
}

const uint8_t* ArchiveAsset::GetAssetData(std::string AssetPath)
{
	for (auto m : NameIndexMap)
	{
		if (m.first == AssetPath) {
			return GetAssetData(m.second);
		}
	}
	return nullptr;
}

const size_t ArchiveAsset::GetAssetSize(int64_t index)
{
	return Files[index].SizeUncompressed;
}

const size_t ArchiveAsset::GetAssetSize(std::string AssetPath)
{
	for (auto m : NameIndexMap)
	{
		if (m.first == AssetPath) {
			return GetAssetSize(m.second);
		}
	}

	return 0;
}
