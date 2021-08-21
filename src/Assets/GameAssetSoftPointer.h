#pragma once
#include <string>
#include "GameAsset.h"
#include "../GunGame.h"
#include "../Logging/Logging.h"

class SoftPointer
{
public:
	const std::filesystem::path& GetPath();
	const std::string GetFileExtension();
	std::filesystem::path Path;

	GameAsset* Get_Internal();
};

template<class AssetType>
class GameAssetSoftPointer
{
public:
	GameAssetSoftPointer(std::filesystem::path AssetPath) 
	{
		Check(std::filesystem::exists(AssetPath));
		Internal.Path = AssetPath;
	}


	AssetType* Get() { return dynamic_cast<AssetType*>(Internal.Get_Internal()); }

	AssetType* LoadSynchronous()
	{
		Logging::LogWarning("GameAssetSoftPointer::LoadSynchronous()", "Note: GameAsset memory deallocation needs to be implemented!");

		if (AssetType* Asset = Get())
		{
			return Asset;
		}

		Logging::Log("GameAssetSoftPointer::LoadSynchronous()", "Loading asset " + Internal.Path.string());


		AssetType* LoadedAsset = AssetType::TryLoad(Internal.Path);
		return LoadedAsset;
	}

	const std::filesystem::path& GetPath() { return Internal.GetPath(); }
	const std::string GetFileExtension() { return Internal.GetFileExtension(); };

private:


	SoftPointer Internal;


};
