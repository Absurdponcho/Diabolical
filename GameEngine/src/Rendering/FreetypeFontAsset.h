#pragma once
#include "../Assets/GameAsset.h"
#include <ft2build.h>
#include <freetype/freetype.h>
#include <glm/glm.hpp>
#include <map>

struct FreetypeCharacter {
	unsigned int TextureID = 0;  // ID handle of the glyph texture
	glm::ivec2   Size = glm::ivec2(0, 0);       // Size of glyph
	glm::ivec2   Bearing = glm::ivec2(0, 0);    // Offset from baseline to left/top of glyph
	unsigned int Advance = 0;    // Offset to advance to next glyph

	
	static std::map<char, FreetypeCharacter> Characters;
};


class FreetypeFontAsset : public GameAsset
{
public:
	static FreetypeFontAsset* TryLoad(std::filesystem::path Path);
	const FT_Face& GetFace();
private:
	FT_Face Face = nullptr;
};