#pragma once
#include <vector>
#include "../Types/DString.h"

class DCommandLine
{
public:
	static const DCommandLine& Get();
	static void Init(int argc, char* argv[]);

	static const bool HasArgument(const DString& Argument);
	static const std::vector<DString>& GetArguments();
	static const std::vector<std::tuple<DString, DString>>& GetKeyValuePairs();
	static const DString* GetValue(const DString& Key);
private:
	DCommandLine(int argc, char* argv[]);
	std::vector<DString> Arguments;
	std::vector<std::tuple<DString, DString>> KeyValuePairs;
};