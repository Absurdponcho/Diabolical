#pragma once
#include <vector>
#include "../Types/DString.h"

class DCommandLine
{
public:
	static DCommandLine& Get();
	static void Init(int argc, char* argv[]);

	const std::vector<DString>& GetArguments() const;
	const std::vector<std::tuple<DString, DString>>& GetKeyValuePairs() const;
	const DString* GetValue(const DString& Key) const;
private:
	DCommandLine(int argc, char* argv[]);
	std::vector<DString> Arguments;
	std::vector<std::tuple<DString, DString>> KeyValuePairs;
};