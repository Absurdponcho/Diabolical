#pragma once
#include "../Assets/GameAsset.h"
#include <ft2build.h>
#include <freetype/freetype.h>

class FreetypeFontAsset : public GameAsset
{
public:
	static FreetypeFontAsset* TryLoad(std::filesystem::path Path);
	const FT_Face& GetFace();
private:
	static FT_Library Library;
	FT_Face Face;
};