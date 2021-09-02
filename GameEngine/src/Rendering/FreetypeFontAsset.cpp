#include "FreetypeFontAsset.h"
#include "../Logging/Logging.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <gl/glew.h>

std::map<char, FreetypeCharacter> FreetypeCharacter::Characters;

FreetypeFontAsset* FreetypeFontAsset::TryLoad(std::filesystem::path Path)
{
	if (!std::filesystem::exists(Path))
	{
		LOGERROR("FreetypeFontAsset::TryLoad()", "Could not find asset at path " + Path.string());
		return nullptr;
	}

	FreetypeFontAsset* LoadedAsset = new FreetypeFontAsset();

	FT_Library Library = nullptr;

	LoadedAsset->bDataAssigned = true;
	LoadedAsset->FilePath = Path;
	FT_Error error;
	if (!Library)
	{
		error = FT_Init_FreeType(&Library);
		if (error)
		{
			LOGERROR("FreetypeFontAsset::TryLoad()", "Could not init freetype library");
		}
	}
	error = FT_New_Face(Library,
		Path.string().c_str(),
		0,
		&LoadedAsset->Face);

	if (error == FT_Err_Unknown_File_Format)
	{
		LOGERROR("FreetypeFontAsset::TryLoad()", "FT_Err_Unknown_File_Format");
	}
	else if (error == FT_Err_Invalid_Table)
	{
		LOGERROR("FreetypeFontAsset::TryLoad()", "FT_Err_Invalid_Table");
	}
	else if (error)
	{
		LOGERROR("FreetypeFontAsset::TryLoad()", "Could not generate FT_New_Memory_Face, error code = " + std::to_string(error));
	}

	FT_Set_Pixel_Sizes(LoadedAsset->Face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

	for (unsigned char c = 0; c < 128; c++)
	{
		// load character glyph 
		if (FT_Load_Char(LoadedAsset->Face, c, FT_LOAD_RENDER))
		{
			LOGERROR("FreetypeFontAsset::TryLoad()", "ERROR::FREETYTPE: Failed to load Glyph");
			continue;
		}
		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			LoadedAsset->Face->glyph->bitmap.width,
			LoadedAsset->Face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			LoadedAsset->Face->glyph->bitmap.buffer
		);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// now store character for later use
		FreetypeCharacter character = {
			texture,
			glm::ivec2(LoadedAsset->Face->glyph->bitmap.width, LoadedAsset->Face->glyph->bitmap.rows),
			glm::ivec2(LoadedAsset->Face->glyph->bitmap_left, LoadedAsset->Face->glyph->bitmap_top),
			(unsigned int)LoadedAsset->Face->glyph->advance.x
		};
		FreetypeCharacter::Characters.insert(std::pair<char, FreetypeCharacter>(c, character));
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	FT_Done_Face(LoadedAsset->Face);
	FT_Done_FreeType(Library);

	LoadedGameAssets.push_back(LoadedAsset);

	return LoadedAsset;
}

const FT_Face& FreetypeFontAsset::GetFace()
{
	return Face;
}