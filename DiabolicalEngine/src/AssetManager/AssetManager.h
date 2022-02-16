#pragma once
#include "../Types/DString.h"
#include "../Thread/ThreadsafeContainer.h"
#include "../Types/DVector.h"
#include <unordered_map>

struct RawAsset
{
	DString Filepath;

};

class DAssetManager
{
public:
	static void Initialize();
	static DAssetManager& Get();

private:
	static DAssetManager* AssetManager;

	// Weak pointers to loaded assets are stored here. If they haven't gone out of memory, 
	// the weak pointer can be given as a shared ptr. Otherwise, it will reload as needed
	DThreadsafeContainer<std::unordered_map<DString, std::weak_ptr<RawAsset>>> LoadedAssets;
};

