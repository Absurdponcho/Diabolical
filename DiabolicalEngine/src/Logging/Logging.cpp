#include "Logging.h"
#include <iostream>
#include <assert.h>
#include "DiabolicalEngine.h"
#include "Filesystem/Filesystem.h"
#include "Check.h"
#include <chrono>
#include <ctime>
#include <fstream>

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#endif

DMutexHandle Mutex;
uint8_t LogColor = 15;
std::ofstream LogFile;
DString LogRecap = "";

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
	DScopedMutex ScopedMutex(Mutex);
	LogFile.write(*String, String.Length());
	LogRecap.Append(String);

	SetLogColor(Color);
	std::cout << String;
	SetLogColor(15);
}

void Logging::LogString(DString Prefix, DString Func, int Line, DString String, int Color)
{
	DString LogStr = DString::Format("%s: %s(%i): %s\n", *Prefix, *Func, Line, *String);
	
	DScopedMutex ScopedMutex(Mutex);
	if (!LogFile.is_open())
	{

		std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

		char Buffer[128];
		ctime_s(Buffer, 128, &time);
		DString TimeString(Buffer);

		// Remove the \n with a chopright
		TimeString = TimeString.RemoveEnd(1);

		DString FilePath = DString("Log/Log-").Append(TimeString).Append(".txt");

		FilePath.Replace(':', '-');

		if (!DFileSystem::FileCreate(FilePath, LogFile))
		{
			std::cout << "Could not create log file" << std::endl;
			Check(false);
		}
	}
	
	LogFile.write(*LogStr, LogStr.Length());
	LogRecap.Append(LogStr);

	SetLogColor(Color);
	std::cout << LogStr;
	SetLogColor(15);

}

void Logging::CloseLogFile()
{
	DScopedMutex ScopedMutex(Mutex);
	if (LogFile.is_open())
	{
		LogFile.flush();
		LogFile.close();
	}
}

const DString& Logging::GetLogRecap()
{
	return LogRecap;
}
