#include "TextAsset.h"
#include "GameAssetSoftPointer.h"
#include <fstream>
#include <filesystem>
#include "../Logging/Logging.h"
#include "../EngineMain.h"
#include "ArchiveAsset.h"

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

std::string StringFromMemory(char* MemLoc, size_t StringSize)
{
	std::stringstream str;
	str << MemLoc;
	std::string OutStr = str.str().substr(0, StringSize + 1);
	OutStr[StringSize] = 0;
	return OutStr;
}

TextAsset* TextAsset::TryLoad(std::filesystem::path Path)
{
	size_t ArcLocation = Path.string().find(".arc", 0);
	bool bArc = ArcLocation < Path.string().size();

	if (!std::filesystem::exists(Path) && !bArc)
	{
		LOGERROR("TextAsset::TryLoad()", "Could not find asset at path " + Path.string());
		return nullptr;
	}

	TextAsset* LoadedAsset = new TextAsset();

	if (!bArc)
	{
		LoadedAsset->String = LoadFileToString(Path.string().c_str());
		LoadedAsset->FileSize = std::filesystem::file_size(Path);
		LoadedAsset->AssetData = (uint8_t*)LoadedAsset->String.c_str();
	}
	else
	{
		GameAssetSoftPointer<ArchiveAsset> Arc = GameAssetSoftPointer<ArchiveAsset>(Path.string().substr(0, ArcLocation + 4));
		ArchiveAsset* ArcAsset = Arc.LoadSynchronous();
		std::string ArcPath = Path.string().substr(ArcLocation + 5, Path.string().length() - (ArcLocation + 5));
		LoadedAsset->FileSize = ArcAsset->GetAssetSize(ArcPath);
		LoadedAsset->String = StringFromMemory((char*)ArcAsset->GetAssetData(ArcPath), LoadedAsset->FileSize);
	}
	
	LoadedAsset->bDataAssigned = true;
	LoadedAsset->FilePath = Path;

	LoadedGameAssets.push_back(LoadedAsset);


	return LoadedAsset;
}

const std::string& TextAsset::GetString()
{
	return String;
}