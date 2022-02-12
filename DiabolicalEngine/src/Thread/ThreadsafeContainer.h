#pragma once
#include "ScopedMutex.h"
#include "../Logging/Logging.h"

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

	T* Get() const
	{
		return ValuePtr;
	}

	T* operator->() const
	{
		return Get();
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

	inline DThreadsafeContainerValue<T> Retreive()
	{
		// Once this function is called, it will not be able to be called again
		// until the returned DThreadsafeContainerValue has been destroyed
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