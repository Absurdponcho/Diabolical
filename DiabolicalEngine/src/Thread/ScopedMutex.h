#pragma once
#include <mutex>

class DMutexHandle : private std::mutex
{
public:
	void Lock()
	{
		lock();
	}

	void Unlock()
	{
		unlock();
	}
};

class DScopedMutex
{
public:
	DScopedMutex(DMutexHandle& MutexHandle)
		: MutHandle(&MutexHandle)
	{
		MutHandle->Lock();
	}

	~DScopedMutex()
	{
		MutHandle->Unlock();
	}

private:
	DMutexHandle* MutHandle = nullptr;

};

