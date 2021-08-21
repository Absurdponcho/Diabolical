#include "GameAssetSoftPointer.h"
#include "../Logging/Logging.h"
#include "../GunGame.h"
#include "../Audio/AudioAsset.h"

GameAsset* GameAssetSoftPointer<GameAsset>::LoadSynchronous()
{
	Logging::LogWarning("GameAssetSoftPointer::LoadSynchronous()", "Note: GameAsset memory deallocation needs to be implemented!");

	if (GameAsset* Asset = Get())
	{
		return Asset;
	}

	Logging::Log("GameAssetSoftPointer::LoadSynchronous()", "Loading base asset " + Internal.Path.string());


	GameAsset* LoadedAsset = GameAsset::TryLoad(Internal.Path);
	return LoadedAsset;
}

AudioAsset* GameAssetSoftPointer<AudioAsset>::LoadSynchronous()
{
	Logging::LogWarning("GameAssetSoftPointer::LoadSynchronous()", "Note: GameAsset memory deallocation needs to be implemented!");

	if (AudioAsset* Asset = Get())
	{
		return Asset;
	}

	Logging::Log("GameAssetSoftPointer::LoadSynchronous()", "Loading audio asset " + Internal.Path.string());

	AudioAsset* LoadedAsset = AudioAsset::TryLoad(Internal.Path);
	return LoadedAsset;
}

GameAsset* SoftPointer::Get_Internal()
{
	GameAsset* LoadedAsset = GameAsset::GetIfLoaded(Path);

	//if (LoadedAsset)
	//{
	//	Logging::LogVerbose("GameAssetSoftPointer::Get_Internal()", "Getting Asset " + Path.string() + ", already loaded");
	//}
	//else
	//{
	//	Logging::LogVerbose("GameAssetSoftPointer::Get_Internal()", "Getting Asset " + Path.string() + ", not loaded");
	//}
	return LoadedAsset;
}

const std::filesystem::path& SoftPointer::GetPath()
{
	return Path;
}

const std::string SoftPointer::GetFileExtension()
{
	return Path.extension().string();
}
