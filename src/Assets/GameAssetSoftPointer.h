#pragma once
#include <string>
#include "GameAsset.h"

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
	GameAssetSoftPointer(std::filesystem::path AssetPath);

	AssetType* Get() { return dynamic_cast<AssetType*>(Internal.Get_Internal()); }

	AssetType* LoadSynchronous();

	const std::filesystem::path& GetPath() { return Internal.GetPath(); }
	const std::string GetFileExtension() { return Internal.GetFileExtension(); };

private:


	SoftPointer Internal;


};
