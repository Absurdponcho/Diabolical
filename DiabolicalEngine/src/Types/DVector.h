#pragma once
#include "Check.h"
#include <vector>

template <typename T>
class DVector : private std::vector<T>
{
public:
	using std::vector<T>::vector;
	using std::vector<T>::begin;
	using std::vector<T>::end;
	using std::vector<T>::operator[];

	const inline size_t Size() const 
	{
		return std::vector<T>::size();
	}
		
	const inline size_t SizeBytes() const
	{
		return std::vector<T>::size() * sizeof(T);
	}

	inline bool Contains(const T& Value)
	{
		for (T& Val : *this)
		{
			if (Val == Value) return true;
		}

		return false;
	}

	inline void Add(T&& Value)
	{
		PushBack(Value);
	}

	inline void Add(const T& Value)
	{
		PushBack(Value);
	}

	inline void PushBack(const T& Value)
	{
		std::vector<T>::emplace_back(Value);
	}

	inline void PushBack(T&& Value)
	{
		std::vector<T>::emplace_back(std::move(Value));
	}

	inline void PushFront(T Value)
	{
		InsertAt(0, Value);
	}

	inline const T GetFront() const 
	{
		Check(IsValidIndex(0));
		return operator[](0);
	}

	inline const T GetBack() const
	{
		Check(IsValidIndex(Size()-1));
		return operator[](Size()-1);
	}

	inline bool PopBack(T* RetValue)
	{
		Check(RetValue);
		if (!IsValidIndex(Size() - 1)) return false;
		*RetValue = Get(Size() - 1);
		RemoveAt(Size() - 1);
		return true; 
	}

	inline bool PopFront(T* RetValue)
	{
		Check(RetValue);
		if (!IsValidIndex(0)) return false;
		*RetValue = Get(0);
		RemoveAt(0);
		return true;
	}

	inline void InsertAt(size_t Index, T Value)
	{
		Check (Index <= Size());
		auto IteratorIndex = std::vector<T>::begin() + Index;
		std::vector<T>::insert(IteratorIndex, Value);
	}

	inline void InsertAt(size_t Index, DVector<T>& Values)
	{
		Check(Index <= Size());
		auto IteratorIndex = std::vector<T>::begin() + Index;
		std::vector<T>::insert(IteratorIndex, Values.begin(), Values.end());
	}

	inline void InsertAt(size_t Index, T* Values, size_t ArraySize)
	{
		Check(Index <= Size());
		for (int64_t Idx = 0; Idx < (int64_t)ArraySize; Idx++)
		{
			InsertAt(Index++, Values[Idx]);
		} 
	}

	inline void Append(T Value)
	{
		InsertAt(Size(), Value);
	}

	inline void Append(DVector<T>& Values)
	{
		InsertAt(Size(), Values);
	}

	inline void Append(T* Values, size_t ArraySize)
	{
		InsertAt(Size(), Values, ArraySize);
	}
	
	inline void Reserve(size_t Size)
	{
		std::vector<T>::reserve(Size);
	}

	inline void ShrinkToFit()
	{
		std::vector<T>::shrink_to_fit();
	}

	inline const size_t Capacity() const
	{
		return std::vector<T>::capacity();
	}

	inline const bool IsValidIndex(size_t Index) const 
	{
		return Index < Size();
	}

	inline int Remove(const T& Value)
	{
		int I = 0;
		for (T& Val : *this)
		{
			if (Val == Value) 
			{
				RemoveAt(I);
				return I;
			}
			I++;
		}

		return -1;
	}

	inline void RemoveAt(size_t Index)
	{
		Check(IsValidIndex(Index));

		std::vector<T>::erase(std::vector<T>::begin() + Index);
	}

	inline void RemoveAt(size_t Index, size_t Count)
	{
		Check(IsValidIndex(Index));

		std::vector<T>::erase(std::vector<T>::begin() + Index, std::vector<T>::begin() + Index + Count);
	}

	inline void Clear()
	{
		std::vector<T>::clear();
	}

	inline const bool IsEmpty() const 
	{
		return Size() == 0;
	}

	inline T Get(size_t Index)
	{
		Check (IsValidIndex(Index));
		return std::vector<T>::operator[](Index);
	}

	inline T* GetData()
	{
		return std::vector<T>::data();
	}
};
