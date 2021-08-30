#include "FreetypeFontAsset.h"
#include "../Logging/Logging.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

FT_Library FreetypeFontAsset::Library;

FreetypeFontAsset* FreetypeFontAsset::TryLoad(std::filesystem::path Path)
{
	if (!std::filesystem::exists(Path))
	{
		Logging::LogError("FreetypeFontAsset::TryLoad()", "Could not find asset at path " + Path.string());
		return nullptr;
	}

	FreetypeFontAsset* LoadedAsset = new FreetypeFontAsset();

	LoadedAsset->bDataAssigned = true;
	LoadedAsset->FilePath = Path;

	FT_Error error = FT_Init_FreeType(&Library);
	if (error)
	{
		Logging::LogError("FreetypeFontAsset::TryLoad()", "Could not init freetype library");
	}

	error = FT_New_Face(Library,
		Path.string().c_str(),
		0,
		&LoadedAsset->Face);

	if (error == FT_Err_Unknown_File_Format)
	{
		Logging::LogError("FreetypeFontAsset::TryLoad()", "FT_Err_Unknown_File_Format");
	}
	else if (error == FT_Err_Invalid_Table)
	{
		Logging::LogError("FreetypeFontAsset::TryLoad()", "FT_Err_Invalid_Table");
	}
	else if (error)
	{
		Logging::LogError("FreetypeFontAsset::TryLoad()", "Could not generate FT_New_Memory_Face, error code = " + std::to_string(error));
	}

	LoadedGameAssets.push_back(LoadedAsset);

	return LoadedAsset;
}

const FT_Face& FreetypeFontAsset::GetFace()
{
	return Face;
}