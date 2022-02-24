#pragma once
#include <thread>
#include <chrono>
#include "GameThread.h"
#include "Logging/Logging.h"
#include "Check.h"

class DThread
{
public:
	template<typename F, typename... Args>
	static DThread& MakeManagedThread(F&& Func, Args... Arguments)
	{
		DThread* Thread = new DThread(Func, Arguments...);
		ManagedThreads.PushBack(Thread);
		return *Thread;
	}

	template<typename F, typename... Args>
	DThread(F&& Func, Args... Arguments)
	{
		Thread = new std::thread([=]()
		{
			Func( Arguments...);
			bComplete = true;
			
		});
	}

	~DThread()
	{
		if (!IsComplete())
		{
			LOG_ERR("DThread destructor called while thread not complete");
		}
		Check(IsComplete());

		Check(Thread);
		delete(Thread);
	}

	bool IsComplete()
	{
		return bComplete;
	}

	void Join()
	{
		if (bJoined) return;
		Check(Thread);
		if (Thread)
		{
			Thread->join();
			bJoined = true;
		}
	}

	static void Sleep(int Milliseconds)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(Milliseconds));
	}

	static void CheckManagedThreads();

	static void TestThreadFunctions();

private:
	static DVector<DThread*> ManagedThreads;
	std::thread* Thread = nullptr;
	bool bComplete = false;
	bool bJoined = false;
};

