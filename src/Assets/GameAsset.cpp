#include "GameAsset.h"
#include "GameAssetSoftPointer.h"
#include <fstream>
#include <filesystem>
#include "../Logging/Logging.h"
#include "../GunGame.h"

std::vector<GameAsset*> GameAsset::LoadedGameAssets;

GameAsset::GameAsset()
{

}

GameAsset::~GameAsset()
{
	if (bDataAssigned)
	{
		delete (AssetData);
	}

	for (int Index = 0; Index < LoadedGameAssets.size(); Index++)
	{
		if (std::filesystem::equivalent(LoadedGameAssets[Index]->FilePath, FilePath))
		{
			LoadedGameAssets.erase(LoadedGameAssets.begin() + Index);
			break;
		}
	}
}

GameAsset* GameAsset::GetIfLoaded(std::filesystem::path Path)
{
	for (int Index = 0; Index < LoadedGameAssets.size(); Index++)
	{
		if (std::filesystem::equivalent(LoadedGameAssets[Index]->FilePath, Path))
		{
			return LoadedGameAssets[Index];
		}
	}

	return nullptr;
}

GameAsset* GameAsset::TryLoad(std::filesystem::path Path)
{
	if (!std::filesystem::exists(Path))
	{
		Logging::LogError("GameAsset::TryLoad()", "Could not find asset at path " + Path.string());
		return nullptr;
	}

	Check(!GetIfLoaded(Path));

	std::ifstream FileStream(Path.string());

	GameAsset* LoadedAsset = new GameAsset();

	LoadedAsset->FileSize = std::filesystem::file_size(Path);

	LoadedAsset->bDataAssigned = true;
	LoadedAsset->AssetData = new uint8_t[LoadedAsset->FileSize];

	FileStream.read((char*)LoadedAsset->AssetData, LoadedAsset->FileSize);

	LoadedGameAssets.push_back(LoadedAsset);
	return LoadedAsset;
}

const uint8_t* GameAsset::GetAssetData()
{
	return AssetData;
}

const size_t GameAsset::GetAssetSize()
{
	return FileSize;;
}