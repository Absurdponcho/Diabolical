#pragma once
#include <functional>
#include "ScopedMutex.h"
#include "../Logging/Logging.h"

namespace SeqHelper
{
	template <int... Is>
	struct index {};

	template <int N, int... Is>
	struct gen_seq : gen_seq<N - 1, N - 1, Is...> {};

	template <int... Is>
	struct gen_seq<0, Is...> : index<Is...> {};
}

// virtual class for polymorphic storage
class DActionBase 
{
public:
	virtual void Run() {}
	virtual ~DActionBase() {}

} ;

template <typename retval, typename... Args>
class DCallback : public DActionBase
{
protected:
	std::function<retval(Args...)> f;
	std::tuple<Args...> args;
public:
	DCallback() {};

	template <typename F, typename... Args>
	DCallback(F&& func, Args&&... args)
		: f(std::forward<F>(func)),
		args(std::forward<Args>(args)...),
		bBound(true)
	{}

	template <typename... Args, int... Is>
	void func(std::tuple<Args...>& tup, SeqHelper::index<Is...>)
	{
		f(std::get<Is>(tup)...);
	}

	template <typename... Args>
	void func(std::tuple<Args...>& tup)
	{
		func(tup, SeqHelper::gen_seq<sizeof...(Args)>{});
	}

public:
	virtual retval Run() override
	{
		if (!bBound)
		{
			LOG_WARN("Running unbound DCallback");
			return (retval)0;
		}
		return func(args);
	}

	template <typename... Args>
	retval Invoke(Args&&... params)
	{
		if (!bBound)
		{
			LOG_WARN("Running unbound DCallback");
			return (retval)0;
		}
		std::tuple<Args...> NewArgs(std::forward<Args>(params)...);
		return func(NewArgs);
	}


	const bool IsBound() const { return bBound; }

protected:
	bool bBound = false;
};

template <typename... Args>
class DAction : public DActionBase
{
protected:
	std::function<void(Args...)> f;
	std::tuple<Args...> args;
	
public:
	DAction() {};

	template <typename F, typename... Args>
	DAction(F&& func, Args&&... args)
		: f(std::forward<F>(func)),
		args(std::forward<Args>(args)...),
		bBound(true)
	{}

	template <typename... Args, int... Is>
	void func(std::tuple<Args...>& tup, SeqHelper::index<Is...>)
	{
		f(std::get<Is>(tup)...);
	}
	 
	template <typename... Args>
	void func(std::tuple<Args...>& tup)
	{
		func(tup, SeqHelper::gen_seq<sizeof...(Args)>{});
	}

public:
	virtual void Run() override
	{
		if (!bBound)
		{
			LOG_WARN("Running unbound DAction");
			return;
		}
		func(args);
	}

	template <typename... Args>
	void Invoke(Args&&... params)
	{
		if (!bBound)
		{
			LOG_WARN("Invoke unbound DAction");
			return;
		}
		std::tuple<Args...> NewArgs(std::forward<Args>(params)...);
		func(NewArgs);
	}


	const bool IsBound() const { return bBound; }

protected:
	bool bBound = false;
};


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

		LaterFuncs.clear();

	}

private:
	static DMutexHandle Mutex;
	static std::vector<DActionBase*> LaterFuncs;

};

