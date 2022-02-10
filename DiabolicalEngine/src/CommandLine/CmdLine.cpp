#include "CmdLine.h"
#include "../Check.h"

DCommandLine* CommandLine = nullptr;

DCommandLine::DCommandLine(int argc, char* argv[])
{
	for (int Index = 0; Index < argc; Index++)
	{
		Arguments.push_back(DString(argv[Index]));
	}

	for (const DString& String : Arguments)
	{
		int Index = String.FindFirst("=");
		if (Index == -1) continue;
		DString Left, Right;
		String.Split(Index, Left, Right);
		KeyValuePairs.push_back(std::make_tuple(Left, Right));
	}
}

const bool DCommandLine::HasArgument(const DString& Argument)
{
	for (const DString& String : Get().GetArguments())
	{
		if (String == Argument)
		{
			return true;
		}
	}
	return false;
}

const std::vector<DString>& DCommandLine::GetArguments()
{
	return Get().Arguments;
}

const std::vector<std::tuple<DString, DString>>& DCommandLine::GetKeyValuePairs()
{
	return Get().KeyValuePairs;
}

const DString* DCommandLine::GetValue(const DString& Key)
{
	for (auto& Tuple : Get().GetKeyValuePairs())
	{
		if (std::get<0>(Tuple).Equals(Key))
		{
			return & std::get<1>(Tuple);
		}
	}

	return nullptr;
}

void DCommandLine::Init(int argc, char* argv[])
{
	CommandLine = new DCommandLine(argc, argv);
}

const DCommandLine& DCommandLine::Get()
{
	Check(CommandLine);
	return *CommandLine;
}

