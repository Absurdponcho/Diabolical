#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>


class GameAsset
{
public:
	static GameAsset* GetIfLoaded(std::filesystem::path Path);
	static GameAsset* TryLoad(std::filesystem::path Path);

	GameAsset();
	virtual ~GameAsset();

	const uint8_t* GetAssetData();
	const size_t GetAssetSize();

protected:
	std::filesystem::path FilePath;

	bool bDataAssigned = false;
	uint8_t* AssetData = nullptr;
	size_t FileSize = 0;

	static std::vector<GameAsset*> LoadedGameAssets;
};