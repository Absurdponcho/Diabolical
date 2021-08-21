#include "GameAssetSoftPointer.h"
#include "../Logging/Logging.h"

GameAsset* GameAssetSoftPointer::LoadSynchronous()
{
	Logging::LogWarning("GameAssetSoftPointer::LoadSynchronous()", "Note: GameAsset memory deallocation needs to be implemented!");

	if (GameAsset* Asset = Get<GameAsset>())
	{
		return Asset;
	}

	Logging::Log("GameAssetSoftPointer::LoadSynchronous()", "Loading asset " + Path.string());


	GameAsset* LoadedAsset = GameAsset::TryLoad(Path);
	return LoadedAsset;
}

GameAssetSoftPointer::GameAssetSoftPointer(std::filesystem::path AssetPath)
{
	Path = AssetPath;
}

GameAsset* GameAssetSoftPointer::Get_Internal()
{
	GameAsset* LoadedAsset = nullptr;

	if (LoadedAsset)
	{
		Logging::LogVerbose("GameAssetSoftPointer::Get_Internal()", "Getting Asset " + Path.string() + ", already loaded");
	}
	else
	{
		Logging::LogVerbose("GameAssetSoftPointer::Get_Internal()", "Getting Asset " + Path.string() + ", not loaded");
	}
	return LoadedAsset;
}