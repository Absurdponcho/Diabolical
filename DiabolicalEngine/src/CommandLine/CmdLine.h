#pragma once
#include <vector>
#include "Types/DString.h"

class DCommandLine
{
public:
	static const DCommandLine& Get();
	static void Init(int argc, char* argv[]);

	static const bool HasArgument(const DString& Argument);
	static const DVector<DString>& GetArguments();
	static const DVector<std::tuple<DString, DString>>& GetKeyValuePairs();
	static const DString* GetValue(const DString& Key);

private:
	DCommandLine(int argc, char* argv[]);
	DVector<DString> Arguments;
	DVector<std::tuple<DString, DString>> KeyValuePairs;
};