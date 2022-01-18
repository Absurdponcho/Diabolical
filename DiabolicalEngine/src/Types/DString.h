#pragma once
#include <string>
#include <iostream>
#include <stdarg.h>
#include <vector>
#include "../Check.h"

class DString : private std::string
{
public:
	using std::string::string;
	using std::string::c_str;

	DString(int Val)
	{
		Append(DString(std::to_string(Val)));
	}

	inline const int FindFirst(const DString& String) const
	{
		size_t Index = find(String);
		if (Index == std::string::npos) return -1;
		else return (int)Index;
	}

	inline const std::vector<size_t> FindAll(const DString& String) const
	{
		std::vector<size_t> positions;
		size_t pos = find(String, 0);
		while (pos != std::string::npos)
		{
			positions.push_back(pos);
			pos = find(String, pos + 1);
		}
		return positions;
	}

	inline const int FindLast(const DString& String) const
	{
		std::vector<size_t> positions = FindAll(String);

		if (positions.size() == 0) return -1;
		else return (int)positions[positions.size() - 1];
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
};