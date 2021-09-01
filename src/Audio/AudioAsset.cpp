#include "AudioAsset.h"
#include "../Logging/Logging.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>


AudioAsset* AudioAsset::TryLoad(std::filesystem::path Path)
{
	if (!std::filesystem::exists(Path))
	{
		LOGERROR("AudioAsset::TryLoad()", "Could not find asset at path " + Path.string());
		return nullptr;
	}

	AudioAsset* LoadedAsset = new AudioAsset();

	LoadedAsset->bDataAssigned = true;
	LoadedAsset->FilePath = Path;

	LoadedAsset->MixChunk = Mix_LoadWAV((char*)Path.u8string().c_str());

	LoadedAsset->AssetData = LoadedAsset->MixChunk->abuf;
	LoadedAsset->FileSize = LoadedAsset->MixChunk->alen;

	LoadedGameAssets.push_back(LoadedAsset);

	return LoadedAsset;
}

Mix_Chunk* AudioAsset::GetMixChunk()
{
	return MixChunk;
}
