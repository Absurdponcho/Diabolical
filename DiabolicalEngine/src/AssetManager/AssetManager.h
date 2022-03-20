#pragma once
#include "Types/DString.h"
#include "Thread/ThreadsafeContainer.h"
#include "Types/DVector.h"
#include <unordered_map>
#include "Thread/GameThread.h"
#include "Types/Action.h"
#include "Thread/Thread.h"
#include "Logging/Logging.h"
#include "Types/DMemory.h"

typedef DAction<DSharedPtr<class DRawAsset>> AAsyncAssetLoad;

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

	DString AsString() const
	{
		DString RetVal = DString();
		RetVal.Append(GetData<char>(), GetAssetSize() + 1);
		RetVal[RetVal.Length()-1] = 0;
		return RetVal;
	}

	DRawAsset(DString NewFilepath, void* NewData, uint64_t NewAssetSize)
		: Data(NewData), Filepath(NewFilepath), AssetSize(NewAssetSize)
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
	DSharedPtr<DRawAsset> SynchronousLoadAsset(DString FilePath);
private:
	DSharedPtr<DRawAsset> Internal_SynchronousLoadAsset(DString& FilePath);

	static DAssetManager* AssetManager;

	DThreadsafeContainer<DVector<PendingAssetLoad>> PendingAssetLoads;
	DUniquePtr<DThread> IOThread;
	void ThreadRun();
	bool bRunning = true;

	// Weak pointers to loaded assets are stored here. If they haven't gone out of memory, 
	// the weak pointer can be given as a shared ptr. Otherwise, it will reload as needed
	std::unordered_map<DString, DWeakPtr<DRawAsset>> LoadedAssets;
};

