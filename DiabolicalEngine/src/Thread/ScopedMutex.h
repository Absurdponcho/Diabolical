#pragma once
#include "Check.h"
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
		Check (MutHandle);
		MutHandle->Lock();
	}

	~DScopedMutex()
	{
		Check(MutHandle);
		MutHandle->Unlock();
	}

private:
	DMutexHandle* MutHandle = nullptr;

};

