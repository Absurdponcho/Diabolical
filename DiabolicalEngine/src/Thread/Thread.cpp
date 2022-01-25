#include "Thread.h"
#include "../Logging/Logging.h"

std::vector<DThread*> DThread::ManagedThreads;

void DThread::CheckManagedThreads()
{
	for (int Index = (int)ManagedThreads.size() - 1; Index >= 0; Index--)
	{
		if (!ManagedThreads[Index])
		{
			ManagedThreads.erase(ManagedThreads.begin() + Index);
			continue;
		}
		if (ManagedThreads[Index]->IsComplete())
		{
			ManagedThreads[Index]->Join();
			delete(ManagedThreads[Index]);
			ManagedThreads.erase(ManagedThreads.begin() + Index);
			LOG("Managed DThread ended successfully");
		}
	}

}

void DThread::TestThreadFunctions()
{
	DThread Thread1 = DThread([]()
	{
		LOG("Hello from thread1");
	});

	DThread Thread2 = DThread([]()
	{
		LOG("Hello from thread2");

		for (int i = 0; i < 10; i++)
		{
			LOG("Sleep thread2");
			DThread::Sleep(80);
		}
	});


	DThread& ManagedDThread = DThread::MakeManagedThread([]()
	{
		LOG("Hello from thread3");

		for (int i = 0; i < 20; i++)
		{
			LOG("Sleep thread3");
			DThread::Sleep(40);
		}
	});

	Thread1.Join();
	Thread2.Join();
	ManagedDThread.Join();
}
