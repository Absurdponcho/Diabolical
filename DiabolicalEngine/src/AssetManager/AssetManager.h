#pragma once
#include "../Types/DString.h"
#include "../Thread/ThreadsafeContainer.h"
#include "../Types/DVector.h"
#include <unordered_map>
#include "../Thread/GameThread.h"
#include "../Types/Action.h"
#include "../Thread/Thread.h"
#include "../Logging/Logging.h"

typedef DAction<std::shared_ptr<class DRawAsset>> AAsyncAssetLoad;

class DRawAsset
{
public:
	// only allow pointers to data to be const. It should not be modified by anything
	template <typename T> 
	const T* GetData() const
	{
		return (const T*)Data;
	}

	const DString& GetFilePath() const
	{
		return Filepath;
	}

	const bool IsValid() const
	{
		return Filepath.Length() && Data;
	}

	const uint64_t GetAssetSize() const
	{
		return AssetSize;
	}

	DRawAsset(DString NewFilepath, void* NewData, uint64_t NewAssetSize)
		: Filepath(NewFilepath), Data(NewData), AssetSize(NewAssetSize)
	{
		LOG(DString::Format("Raw Asset Loaded. %s", *NewFilepath));
	}

	DRawAsset() {}	

	~DRawAsset()
	{
		LOG(DString::Format("Raw Asset Destroyed. %s", *Filepath));
		if (Data)
		{
			free (Data);
		}
	}

protected:
	void* Data = nullptr;
	DString Filepath;
	uint64_t AssetSize = 0;
};

struct PendingAssetLoad
{
	DString Filepath;
	AAsyncAssetLoad Action;
};

class DAssetManager
{
public:
	DAssetManager();
	~DAssetManager();

	static void Initialize();
	static DAssetManager& Get();

	void AsyncLoadAsset(DString FilePath, AAsyncAssetLoad OnAssetLoad);
	std::shared_ptr<DRawAsset> SynchronousLoadAsset(DString FilePath);
private:
	std::shared_ptr<DRawAsset> Internal_SynchronousLoadAsset(DString FilePath);

	static DAssetManager* AssetManager;

	DThreadsafeContainer<DVector<PendingAssetLoad>> PendingAssetLoads;
	std::unique_ptr<DThread> IOThread;
	void ThreadRun();
	bool bRunning = true;

	// Weak pointers to loaded assets are stored here. If they haven't gone out of memory, 
	// the weak pointer can be given as a shared ptr. Otherwise, it will reload as needed
	std::unordered_map<DString, std::weak_ptr<DRawAsset>> LoadedAssets;
};

