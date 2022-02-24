#pragma once
#include <string>
#include <iostream>
#include <stdarg.h>
#include <vector>
#include "Check.h"
#include <algorithm>

#include <limits>
#include <stdint.h>
#include "DVector.h"


template <typename T, typename U>
bool CanTypeFitValue(const U value) {
	constexpr intmax_t botT = intmax_t(std::numeric_limits<T>::min());
	constexpr intmax_t botU = intmax_t(std::numeric_limits<U>::min());
	constexpr uintmax_t topT = uintmax_t(std::numeric_limits<T>::max());
	constexpr uintmax_t topU = uintmax_t(std::numeric_limits<U>::max());
	return !((botT > botU && value < static_cast<U> (botT)) || (topT < topU&& value > static_cast<U> (topT)));
}

class DString : private std::string
{
public:
	using std::string::string;
	using std::string::operator[];

	DString(int Val)
	{
		Append(DString(std::to_string(Val)));
	}

	inline std::string ToSTLString() const
	{
		return std::string(**this);
	}

	inline const bool TryParseLong(long& Value) const
	{
		char* end;
		long NewValue = strtol(**this, &end, 10);
		if (end == **this || *end != '\0' || errno == ERANGE)
		{
			return false;
		}

		Value = NewValue;
		return true;
	}

	inline const bool TryParseInt(int& Value) const
	{
		long TestValue = 0;
		if (TryParseLong(TestValue))
		{
			if (CanTypeFitValue<int>(TestValue))
			{
				Value = (int)TestValue;
				return true;
			}
		}

		return false;
	}

	inline const bool TryParseShort(short& Value) const
	{
		long TestValue = 0;
		if (TryParseLong(TestValue))
		{
			if (CanTypeFitValue<short>(TestValue))
			{
				Value = (short)TestValue;
				return true;
			}
		}

		return false;
	}

	inline const bool TryParseByte(char& Value) const
	{
		long TestValue = 0;
		if (TryParseLong(TestValue))
		{
			if (CanTypeFitValue<char>(TestValue))
			{
				Value = (char)TestValue;
				return true;
			}
		}

		return false;
	}

	inline const int FindFirst(const DString& String) const
	{
		size_t Index = find(String);
		if (Index == std::string::npos) return -1;
		else return (int)Index;
	}

	inline const DVector<size_t> FindAll(const DString& String) const
	{
		DVector<size_t> positions;
		size_t pos = find(String, 0);
		while (pos != std::string::npos)
		{
			positions.PushBack(pos);
			pos = find(String, pos + 1);
		}
		return positions;
	}

	inline const int FindLast(const DString& String) const
	{
		DVector<size_t> positions = FindAll(String);

		if (positions.Size() == 0) return -1;
		else return (int)positions[positions.Size() - 1];
	}

	inline void Split(int Index, DString& Left, DString& Right) const
	{
		Check(Index > 0);
		Check(Index < Length());
		if (Index <= 0) return;
		if (Index >= Length()) return;
		Left = DString(substr(0, Index));
		Right = DString(substr(Index+1, Length()));
	}

	inline bool Split(const DString& Delimiter, DString& Left, DString& Right) const
	{
		int Index = FindFirst(Delimiter);
		if (Index <= 0) return false;
		if (Index >= Length()) return false;
		Left = DString(substr(0, Index));
		Right = DString(substr(Index + Delimiter.Length(), Length()));

		return true;
	}

	inline const size_t Length() const
	{
		return length();
	}

	inline bool Equals(const DString& Other) const
	{
		return *this == Other;
	}

	inline DString& Append(const DString& Other)
	{
		append(Other);	
		return *this;
	}

	inline DString& Append(const char* String, size_t StrLength)
	{
		append(String, StrLength);
		return *this;
	}

	inline friend bool operator==(const DString& LHS, const DString& RHS)
	{
		return LHS._Equal(*RHS);
	}

	inline friend bool operator!=(const DString& LHS, const DString& RHS)
	{
		return !LHS._Equal(*RHS);
	}

	inline friend std::ostream& operator<<(std::ostream& os, const DString& String)
	{
		os << String.c_str();
		return os;
	}

	template<typename... Args>
	static DString Format(DString Format, Args... args)
	{
		const int BufferSize = 4096 * 2;
		char Buffer[BufferSize];

		snprintf(Buffer, BufferSize, Format.c_str(), args...);

		return DString(Buffer);
	}

	inline const char* operator*() const
	{
		return c_str();
	}

	// Remove "Count" characters from the end of the string
	inline DString RemoveEnd(size_t Count)
	{
		return RemoveRange(Length()-Count, Count);
	}

	// Remove "Count" characters from the start of the string
	inline DString RemoveStart(size_t Count)
	{
		return RemoveRange(0, Count);
	}

	inline DString RemoveRange(size_t Index, size_t Count) const 
	{
		Check(Count + Index < Length() + 1);
		DString RetStr = *this;
		RetStr.erase(Index, Count);
		return RetStr;
	}

	inline const bool EndsWith(const DString& Str) const 
	{
		return FindLast(Str) == Length() - Str.Length();
	}

	inline const bool StartsWith(const DString& Str) const
	{
		return FindFirst(Str) == 0;
	}

	inline void Replace(char Char, char Replacement) 
	{
		std::replace(begin(), end(), Char, Replacement);
	}

};

namespace std {

	template <>
	struct hash<DString>
	{
		std::size_t operator()(const DString& k) const
		{
			// use std::string hash
			std::hash<std::string> ha; 
			return ha.operator()(k.ToSTLString());			
		}
	};

}