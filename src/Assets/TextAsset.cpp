#include "TextAsset.h"
#include "GameAssetSoftPointer.h"
#include <fstream>
#include <filesystem>
#include "../Logging/Logging.h"
#include "../GunGame.h"

std::string LoadFileToString(char const* const fname)
{
	std::ifstream ifile(fname);
	std::string filetext;

	while (ifile.good()) {
		std::string line;
		std::getline(ifile, line);
		filetext.append(line + "\n");
	}

	return filetext;
}

TextAsset* TextAsset::TryLoad(std::filesystem::path Path)
{
	if (!std::filesystem::exists(Path))
	{
		Logging::LogError("GameAsset::TryLoad()", "Could not find asset at path " + Path.string());
		return nullptr;
	}

	
	TextAsset* LoadedAsset = new TextAsset();

	LoadedAsset->String = LoadFileToString(Path.string().c_str());

	LoadedAsset->FileSize = std::filesystem::file_size(Path);

	LoadedAsset->bDataAssigned = true;
	LoadedAsset->AssetData = (uint8_t*)LoadedAsset->String.c_str();
	LoadedAsset->FilePath = Path;

	LoadedGameAssets.push_back(LoadedAsset);


	return LoadedAsset;
}

const std::string& TextAsset::GetString()
{
	return String;
}