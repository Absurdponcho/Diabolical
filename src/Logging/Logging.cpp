#include "Logging.h"
#include <iostream>
#include "../GunGame.h"
#include "../Utility/Utility.h"

#ifdef PLATFORM_WINDOWS
	#include <windows.h>
#endif

ELogVerbosity Logging::LogVerbosity = ELogVerbosity::LV_Default;
uint8_t Logging::LogColor = 15;

void Logging::Log(const std::wstring& DebugScope, const std::wstring& DebugString)
{
	if (LogVerbosity >= ELogVerbosity::LV_Default)
	{
		uint8_t OldColor = LogColor;
		SetLogColor(15);
		std::wcout << DebugScope << ": " << DebugString << std::endl;
		SetLogColor(OldColor);
	}
}

void Logging::LogVerbose(const std::wstring& DebugScope, const std::wstring& DebugString)
{
	if (LogVerbosity >= ELogVerbosity::LV_Verbose)
	{
		uint8_t OldColor = LogColor;
		SetLogColor(10);
		std::wcout << DebugScope << ": " << DebugString << std::endl;
		SetLogColor(OldColor);
	}
}

void Logging::LogWarning(const std::wstring& DebugScope, const std::wstring& DebugString)
{
	if (LogVerbosity >= ELogVerbosity::LV_WarningOnly)
	{
		uint8_t OldColor = LogColor;
		SetLogColor(14);
		std::wcout << DebugScope << ": " << DebugString << std::endl;
		SetLogColor(OldColor);
	}
}

void Logging::LogError(const std::wstring& DebugScope, const std::wstring& DebugString)
{
	if (LogVerbosity >= ELogVerbosity::LV_ErrorOnly)
	{
		uint8_t OldColor = LogColor;
		SetLogColor(12);
		std::wcout << DebugScope << ": " << DebugString << std::endl;
		SetLogColor(OldColor);

	}
}

void Logging::Log(const std::string& DebugScope, const std::string& DebugString)
{
	Log(Utility::StringToWString(DebugScope), Utility::StringToWString(DebugString));
}

void Logging::LogVerbose(const std::string& DebugScope, const std::string& DebugString)
{
	LogVerbose(Utility::StringToWString(DebugScope), Utility::StringToWString(DebugString));
}

void Logging::LogWarning(const std::string& DebugScope, const std::string& DebugString)
{
	LogWarning(Utility::StringToWString(DebugScope), Utility::StringToWString(DebugString));
}

void Logging::LogError(const std::string& DebugScope, const std::string& DebugString)
{
	LogError(Utility::StringToWString(DebugScope), Utility::StringToWString(DebugString));
}

void Logging::SetLogVerbosity(ELogVerbosity Verbosity)
{
	LogVerbosity = Verbosity;
}

ELogVerbosity Logging::GetLogVerbosity()
{
	return LogVerbosity;
}

void Logging::SetLogColor(uint8_t Color)
{
	LogColor = Color;
#ifdef PLATFORM_WINDOWS
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, Color);
#else
	if (LogVerbosity == ELogVerbosity::LV_Default)
	{
		std::cout << "Logging::SetLogColor(): Unimplemented platform";
	}
#endif
}
