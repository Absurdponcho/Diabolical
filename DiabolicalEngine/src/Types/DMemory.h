#pragma once
#include <memory>
#include "ECS/ECS.h"

template <typename T>
class DWeakPtr;

template <typename T>
class DSharedPtr : private std::shared_ptr<T>
{
public:
	using std::shared_ptr<T>::operator[];
	using std::shared_ptr<T>::operator->;

	DSharedPtr() {}

	DSharedPtr(T* In) : std::shared_ptr<T>(In) {}

	DSharedPtr(std::shared_ptr<T> Other) : std::shared_ptr<T>(Other) {}

	// constructor to allow casting of the inner type
	template <typename T2>
	DSharedPtr(const DSharedPtr<T2>&& Other) : std::shared_ptr<T>::shared_ptr(Other) {}

	template <typename T2>
	DSharedPtr(const DSharedPtr<T2>& Other) : std::shared_ptr<T>::shared_ptr(Other.AsSTLType()) {}

	template <typename T2>
	DSharedPtr(const DSharedPtr<T2>* Other) : std::shared_ptr<T>::shared_ptr(Other) {}

	const inline std::shared_ptr<T>& AsSTLType() const
	{
		static_assert(sizeof(DSharedPtr<T>) == sizeof(std::shared_ptr<T>));
		return *(std::shared_ptr<T>*)this;
	}

	inline T* Get() const 
	{
		return std::shared_ptr<T>::get();
	}


};

template <typename T>
class DWeakPtr : private std::weak_ptr<T>
{
public:
	DWeakPtr() {};

	DWeakPtr(const DSharedPtr<T>& Shared) : std::weak_ptr<T>::weak_ptr(Shared.AsSTLType()) {}

	inline bool Expired() const 
	{
		return std::weak_ptr<T>::expired();
	}

	inline DSharedPtr<T> Lock()
	{
		return std::weak_ptr<T>::lock();
	}

	inline bool IsValid() const
	{
		return std::weak_ptr<T>::get() != nullptr;
	}

	inline T* Get() const
	{
		return std::weak_ptr<T>::get();
	}
};

template <typename T>
class DUniquePtr : private std::unique_ptr<T>
{
public:
	using std::unique_ptr<T>::operator->;

	DUniquePtr(std::unique_ptr<T> In) : std::unique_ptr<T>(In.release()) {}

	DUniquePtr(T* In) : std::unique_ptr<T>(In) {}

	DUniquePtr() {}

	T* Get() const 
	{
		return std::unique_ptr<T>::get();
	}

	inline void Swap(DUniquePtr& Other)
	{
		std::unique_ptr<T>::swap(Other);
	}

	inline T* Release()
	{
		return std::unique_ptr<T>::release();
	}
};
