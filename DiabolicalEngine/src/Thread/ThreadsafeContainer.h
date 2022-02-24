#pragma once
#include "ScopedMutex.h"
#include "Logging/Logging.h"

template <typename T>
class DThreadsafeContainerValue
{
public:

	DThreadsafeContainerValue(DMutexHandle& MutexHandle, T& Value)
		: MutexPtr(&MutexHandle), ValuePtr(&Value)
	{}

	~DThreadsafeContainerValue()
	{
		MutexPtr->Unlock();
	}

	inline T& Get() const
	{
		return *ValuePtr;
	}

	inline T* GetPtr() const
	{
		return ValuePtr;
	}

	inline T* operator->() const
	{
		return GetPtr();
	}

private:
	DMutexHandle* MutexPtr = nullptr;
	T* ValuePtr = nullptr;
};

// This container holds 1 of a value. It only lets 1 thing access it at a time, through the use of 
// DThreadsafeContainerValue. 
template <typename T>
class DThreadsafeContainer
{

public:
	// If no value is provided, create a value with its default constructor
	DThreadsafeContainer()
		: Value(T())
	{}

	DThreadsafeContainer(T NewValue)
		: Value(NewValue)
	{}

	// Locks the value inside, allowing only 1 thread to access it until the returned DThreadsafeContainerValue is destroyed
	inline DThreadsafeContainerValue<T> Retrieve()
	{
		Mutex.Lock();
		return DThreadsafeContainerValue(Mutex, Value);
	}

	// Sets the value inside this container.
	inline void Set(T NewValue)
	{
		DScopedMutex _(Mutex);
		Value = NewValue;
	}

	// Copies the value inside this container.
	inline T Copy() const
	{
		DScopedMutex _(Mutex);
		return Value;
	}

private:
	DMutexHandle Mutex;
	T Value;
};