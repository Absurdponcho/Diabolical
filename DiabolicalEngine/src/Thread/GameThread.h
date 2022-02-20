#pragma once
#include <functional>
#include "ScopedMutex.h"
#include "../Logging/Logging.h"
#include "../Types/DVector.h"
#include "../Types/Action.h"

class DGameThread
{
public:

	// invoke any DAction at the end of the game loop
	template<typename... Args>
	static void Invoke(DAction<Args...> Action)
	{
		DScopedMutex ScopedMutex(Mutex);
		DAction<Args...>* HeapAction = new DAction<Args...>();
		*HeapAction = Action; // Copy
		LaterFuncs.push_back(HeapAction);
	}

	// invoke any std::function type such as a function or a lambda
	template<typename F, typename... Args>
	static void Invoke(F&& Func, Args... Arguments)
	{
		Invoke(DAction(Func, Arguments...));
	}

	static void RunInvokedFunctions()
	{
		DScopedMutex ScopedMutex(Mutex);

		for (DActionBase* Func : LaterFuncs)
		{
			Func->Run();
			delete (Func);
		}

		LaterFuncs.Clear();

	}

private:
	static DMutexHandle Mutex;
	static DVector<DActionBase*> LaterFuncs;

};

