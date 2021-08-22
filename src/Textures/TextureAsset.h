#pragma once
#include "../Assets/GameAsset.h"
#include "SDL2/SDL_image.h"
#include "../GunGame.h"
class TextureAsset : public GameAsset
{
public:
	static TextureAsset* TryLoad(std::filesystem::path Path);
	SDL_Texture* GetTexture();

	// Since SDL handles the memory here it becomes a bit hard to deal with the 
	// data directly. Maybe we change this some day. It's possible that
	// SDL loads the texture into the GPU and just returns a handle so
	// idk. This works fine for now
	const virtual uint8_t* GetAssetData() { Check(false); return nullptr; };
	const virtual size_t GetAssetSize() { Check(false); return 0; };
private:
	SDL_Texture* SDLTexture;
};

