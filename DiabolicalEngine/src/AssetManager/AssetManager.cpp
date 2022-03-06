#include "AssetManager.h"
#include "Check.h"
#include <iostream>
#include <fstream>
#include "FileSystem/Filesystem.h"

DAssetManager* DAssetManager::AssetManager = nullptr;

DAssetManager::DAssetManager()
	: IOThread(std::make_unique<DThread>([=](){ ThreadRun(); }))
{

}

DAssetManager::~DAssetManager()
{
	bRunning = false;
	IOThread->Join();
}

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

void DAssetManager::AsyncLoadAsset(DString FilePath, AAsyncAssetLoad OnAssetLoad)
{
	if (FilePath.Length() == 0)
	{
		LOG_ERR("FilePath is empty!");
		Check(false);
		return;
	}

	FilePath = DFileSystem::NormalizeFilePath(FilePath);

	Check (OnAssetLoad.IsBound());
	if (!OnAssetLoad.IsBound()) return;

	PendingAssetLoad NewPendingLoad = PendingAssetLoad();
	NewPendingLoad.Filepath = FilePath;
	NewPendingLoad.Action = OnAssetLoad;
	auto Value = PendingAssetLoads.Retrieve();
	Value->Append(NewPendingLoad);
}

void DAssetManager::ThreadRun()
{
	while (bRunning)
	{
		DVector<PendingAssetLoad> CurrentAssetLoads;
		{			
			// retrieve the threadsafe container, drain the contents into CurrentAssetLoads,
			// then do the loading to minimize mutex time
			auto Value = PendingAssetLoads.Retrieve();
			CurrentAssetLoads.Append(Value.Get());
			Value->Clear();
		}

		for (PendingAssetLoad& PendingLoad : CurrentAssetLoads)
		{
			if (PendingLoad.Action.IsBound())
			{
				DSharedPtr<DRawAsset> LoadedAsset = Internal_SynchronousLoadAsset(PendingLoad.Filepath);
				PendingLoad.Action.Invoke(LoadedAsset);
			}
		}

		DThread::Sleep(20);
	}
}

DSharedPtr<DRawAsset> DAssetManager::SynchronousLoadAsset(DString FilePath)
{
	if (FilePath.Length() == 0)
	{
		LOG_ERR("FilePath is empty!");
		Check(false);
		return nullptr;
	}
	// To ensure the async load system does not encounter race conditions with 
	// synchronous loading, just make the synchronous load do an async load ;)
	DString NormalizedFilePath = DFileSystem::NormalizeFilePath(FilePath);

	if (NormalizedFilePath.Length() == 0)
	{
		LOG_ERR(DString::Format("NormalizedFilePath is empty! oldpath: %s", *FilePath));
		Check(false);
		return nullptr;
	}

	bool bLoaded = false;
	DSharedPtr<DRawAsset> LoadedAsset;
	AsyncLoadAsset(NormalizedFilePath, [&](DSharedPtr<DRawAsset> Asset)
	{ 
		LoadedAsset = Asset; 
		bLoaded = true; 	
	});

	while (!bLoaded)
	{
		DThread::Sleep(1);
	}

	return LoadedAsset;
}

DSharedPtr<DRawAsset> DAssetManager::Internal_SynchronousLoadAsset(DString& FilePath)
{
	if (FilePath.Length() == 0)
	{
		LOG_ERR("FilePath is empty!");
		Check(false);
		return nullptr;
	}

	auto AssetIt = LoadedAssets.find(FilePath);
	if (AssetIt != LoadedAssets.end())
	{
		DWeakPtr<DRawAsset> SavedAsset = AssetIt->second;
		if (!SavedAsset.Expired())
		{
			DSharedPtr<DRawAsset> SavedAssetSharedPtr = SavedAsset.Lock();
			if (SavedAssetSharedPtr.Get())
			{
				// asset is still valid in memory somewhere, return it
				return SavedAssetSharedPtr; 
			}
		}
	}

	if (!DFileSystem::PathExists(FilePath))
	{ 
		LOG_ERR(DString::Format("FilePath does not exist: %s", *FilePath));
		Check(false);
		return nullptr;
	}

	uint64_t FileSize = DFileSystem::FileSize(FilePath);
	if (FileSize == 0)
	{
		LOG_ERR("FileSize == 0");
		Check(false);
		return nullptr;
	}

	char* Buffer = (char*)malloc(FileSize);
	Check(Buffer);

	// DRawAsset will handle deletion of Buffer from now
	DSharedPtr<DRawAsset> NewAsset = std::make_shared<DRawAsset>(FilePath, Buffer, FileSize);

	std::ifstream in = std::ifstream(*FilePath, std::ios::binary);
	Check (in.is_open());

	in.read((char*)Buffer, FileSize);

	// store weak reference to the loaded asset
	LoadedAssets[FilePath] = NewAsset;

	return NewAsset;
}
