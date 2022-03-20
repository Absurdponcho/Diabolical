#pragma once
#include "Logging/Logging.h"
#include <functional>

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

};

template <typename retval, typename... Args>
class DCallback : public DActionBase
{
protected:
	std::function<retval(Args...)> f;
	std::tuple<Args...> args;
public:
	DCallback() {};

	template <typename F>
	DCallback(F&& func, Args&&... args)
		: f(std::forward<F>(func)),
		args(std::forward<Args>(args)...),
		bBound(true)
	{}

	template <int... Is>
	void func(std::tuple<Args...>& tup, SeqHelper::index<Is...>)
	{
		f(std::get<Is>(tup)...);
	}

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

	retval Invoke(Args&&... params)
	{
		if (!bBound)
		{
			LOG_WARN("Invoking unbound DCallback");
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
	std::tuple<Args...> args = {};

public:
	DAction() {};

	template <typename F>
	DAction(F&& func, Args&&... args)
		: f(std::forward<F>(func)),
		args(std::forward<Args>(args)...),
		bBound(true)
	{}

	template <typename F>
	DAction(F func)
		: bBound(true)
	{
		f = std::function<void(Args...)>(func);
	}

	template <int... Is>
	void func(std::tuple<Args...>& tup, SeqHelper::index<Is...>) 
	{
		f(std::get<Is>(tup)...);
	}

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

	void Invoke(Args&&... params)
	{
		if (!bBound)
		{
			LOG_WARN("Invoking unbound DAction");
			return;
		}
		std::tuple<Args...> NewArgs(std::forward<Args>(params)...);
		func(NewArgs);
	}

	void Invoke(const Args&... params)
	{
		if (!bBound)
		{
			LOG_WARN("Invoking unbound DAction");
			return;
		}
		std::tuple<Args...> NewArgs(params...);
		func(NewArgs);
	}

	const bool IsBound() const { return bBound; }

protected:
	bool bBound = false;
};