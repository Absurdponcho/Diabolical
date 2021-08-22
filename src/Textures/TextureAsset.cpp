#include "TextureAsset.h"
#include "../Logging/Logging.h"
#include <SDL2/SDL.h>
#include "..\WindowManager.h"

TextureAsset* TextureAsset::TryLoad(std::filesystem::path Path)
{
	SDL_Surface* LoadedSurface = NULL;

	if (!std::filesystem::exists(Path))
	{
		Logging::LogError("GameAsset::TryLoad()", "Could not find asset at path " + Path.string());
		return nullptr;
	}

	std::ifstream FileStream(Path.string());

	TextureAsset* LoadedAsset = new TextureAsset();


	LoadedAsset->bDataAssigned = true;
	LoadedAsset->FilePath = Path;

	LoadedAsset->SDLTexture = IMG_LoadTexture(WindowManager::GetSDLRenderer(), LoadedAsset->FilePath.string().c_str());
	if (LoadedAsset->SDLTexture == nullptr) {
		Logging::LogError("TextureASset::TryLoad()", "IMG_LoadTexture failed");
	}
	LoadedAsset->AssetData = new uint8_t[LoadedAsset->FileSize];
	LoadedAsset->FileSize = std::filesystem::file_size(Path);

	FileStream.read((char*)LoadedAsset->AssetData, LoadedAsset->FileSize);

	LoadedGameAssets.push_back(LoadedAsset);
	return LoadedAsset;
}

SDL_Texture* TextureAsset::GetTexture()
{
	return SDLTexture;
}
