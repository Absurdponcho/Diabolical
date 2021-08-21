#include "GameAssetSoftPointer.h"
#include "../Logging/Logging.h"
#include "../GunGame.h"

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
	Check(std::filesystem::exists(AssetPath));

	Path = AssetPath;
}

GameAsset* GameAssetSoftPointer::Get_Internal()
{
	GameAsset* LoadedAsset = GameAsset::GetIfLoaded(Path);

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

const std::filesystem::path& GameAssetSoftPointer::GetPath()
{
	return Path;
}