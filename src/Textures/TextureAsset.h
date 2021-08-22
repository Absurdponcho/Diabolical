#pragma once
#include "../Assets/GameAsset.h"
#include "SDL2/SDL_image.h"
class TextureAsset : public GameAsset
{
public:
	static TextureAsset* TryLoad(std::filesystem::path Path);
	SDL_Texture* GetTexture();
private:
	SDL_Texture* SDLTexture;
};

