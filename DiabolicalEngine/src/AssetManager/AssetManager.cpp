#include "AssetManager.h"
#include "../Check.h"
#include <iostream>
#include <fstream>
#include "../FileSystem/Filesystem.h"

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
				std::shared_ptr<DRawAsset> LoadedAsset = SynchronousLoadAsset(PendingLoad.Filepath);
				PendingLoad.Action.Invoke(LoadedAsset);
			}
		}

		DThread::Sleep(20);
	}
}

std::shared_ptr<DRawAsset> DAssetManager::SynchronousLoadAsset(DString FilePath)
{
	Check (DFileSystem::PathExists(FilePath));
	if (!DFileSystem::PathExists(FilePath))
	{
		LOG_ERR("!DFileSystem::PathExists(FilePath)");
		return nullptr;
	}

	uint64_t FileSize = DFileSystem::FileSize(FilePath);
	Check (FileSize != 0);
	if (FileSize == 0)
	{
		LOG_ERR("FileSize == 0");
		return nullptr;
	}

	char* Buffer = (char*)malloc(FileSize);
	Check(Buffer);

	// DRawAsset will handle deletion of Buffer from now
	std::shared_ptr<DRawAsset> NewAsset = std::make_shared<DRawAsset>(FilePath, Buffer, FileSize);

	std::ifstream in = std::ifstream(*FilePath, std::ios::binary);
	Check (in.is_open());

	in.read((char*)Buffer, FileSize);

	return NewAsset;
}