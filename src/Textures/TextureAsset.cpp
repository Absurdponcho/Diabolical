#include "TextureAsset.h"
#include "../Logging/Logging.h"
#include <SDL2/SDL.h>
#include "../WindowManager.h"
#include <SOIL/SOIL.h>
#include <gl/glew.h>

TextureAsset* TextureAsset::TryLoad(std::filesystem::path Path)
{
	SDL_Surface* LoadedSurface = NULL;

	if (!std::filesystem::exists(Path))
	{
		LOGERROR("GameAsset::TryLoad()", "Could not find asset at path " + Path.string());
		return nullptr;
	}

	std::ifstream FileStream(Path.string());

	TextureAsset* LoadedAsset = new TextureAsset();

	LoadedAsset->bDataAssigned = true;
	LoadedAsset->FilePath = Path;

	LoadedAsset->OpenGLTexture = SOIL_load_OGL_texture(Path.string().c_str(), 4, 0, SOIL_FLAG_MIPMAPS );

	// SOIL loads the texture and sets the filter flags to be filtered (EW!!!).
	// so we need to bind the texture and set them to have no filtering :)
	glBindTexture(GL_TEXTURE_2D, LoadedAsset->OpenGLTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (LoadedAsset->OpenGLTexture == 0) {
		LOGERROR("TextureASset::TryLoad()", "SOIL_load_OGL_texture failed: " + std::string(SOIL_last_result()) + " : " + std::string((char*)Path.c_str()));
		return nullptr;
	}

	LoadedGameAssets.push_back(LoadedAsset);
	return LoadedAsset;
}

unsigned int TextureAsset::GetTexture()
{
	return OpenGLTexture;
}
