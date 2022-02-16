#include "AssetManager.h"
#include "../Check.h"

DAssetManager* DAssetManager::AssetManager = nullptr;

void DAssetManager::Initialize()
{
	AssetManager = new DAssetManager();
	Check (AssetManager);
}

DAssetManager& DAssetManager::Get()
{
	Check (AssetManager);
	return *AssetManager;
}
