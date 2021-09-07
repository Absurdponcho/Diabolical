#pragma once
#include <string>
#include "GameAsset.h"
#include "../EngineMain.h"
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
		LOG("", AssetPath.string());
		Internal.Path = AssetPath;
	}


	AssetType* Get() { return dynamic_cast<AssetType*>(Internal.Get_Internal()); }

	AssetType* LoadSynchronous()
	{

		if (AssetType* Asset = Get())
		{
			return Asset;
		}

		LOGWARNING("GameAssetSoftPointer::LoadSynchronous()", "Note: GameAsset memory deallocation needs to be implemented!");
		LOG("GameAssetSoftPointer::LoadSynchronous()", "Loading asset " + Internal.Path.string());


		AssetType* LoadedAsset = AssetType::TryLoad(Internal.Path);
		return LoadedAsset;
	}

	const std::filesystem::path& GetPath() { return Internal.GetPath(); }
	const std::string GetFileExtension() { return Internal.GetFileExtension(); };

private:


	SoftPointer Internal;


};
