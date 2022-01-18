#include "Logging.h"
#include <iostream>
#include <assert.h>
#include "../DiabolicalEngine.h"
#include "../Filesystem/Filesystem.h"
#include "../Check.h"

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#endif

uint8_t LogColor = 15;
std::ofstream LogFile;

void Logging::SetLogColor(uint8_t Color)
{
	LogColor = Color;
#ifdef PLATFORM_WINDOWS
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, Color);
#else
	Check(false);
#endif
}

void Logging::LogPlain(DString String, int Color)
{
	LogFile.write(*String, String.Length());

	SetLogColor(Color);
	std::cout << String;
	SetLogColor(15);
}

void Logging::LogString(DString Prefix, DString Func, int Line, DString String, int Color)
{
	DString LogStr = DString::Format("%s: %s(%i): %s\n", *Prefix, *Func, Line, *String);

	if (!LogFile.is_open())
	{
		if (!diabolical::FileCreate(DString("Log/Log-").Append("a").Append(".txt"), LogFile))
		{
			std::cout << "Could not create log file" << std::endl;
			Check(false);
		}
	}
	
	LogFile.write(*LogStr, LogStr.Length());

	SetLogColor(Color);
	std::cout << LogStr;
	SetLogColor(15);

}

void Logging::CloseLogFile()
{
	if (LogFile.is_open())
	{
		LogFile.flush();
		LogFile.close();
	}
}
