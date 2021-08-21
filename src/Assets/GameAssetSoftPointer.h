#pragma once
#include <string>
#include "GameAsset.h"

class GameAssetSoftPointer
{
public:
	GameAssetSoftPointer(std::filesystem::path AssetPath);

	template<class T>
	T* Get() { return dynamic_cast<T*>(Get_Internal()); }

	GameAsset* LoadSynchronous();

private:
	GameAsset* Get_Internal();
	
	std::filesystem::path Path;
};
