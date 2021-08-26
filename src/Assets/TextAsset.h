#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include "GameAsset.h"


class TextAsset : GameAsset
{
public:
	static TextAsset* TryLoad(std::filesystem::path Path);
	const std::string& GetString();
protected:
	std::string String;
};