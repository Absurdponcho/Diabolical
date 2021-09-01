#pragma once
#include <string>

// Example 
// Logging::Log("Class::Function", "Debug String");

#define LOGGING

enum class ELogVerbosity : uint8_t
{
	LV_ErrorOnly = 0,
	LV_WarningOnly = 1,
	LV_Default = 3,
	LV_Verbose = 4, // verbose for really verbose logging
};

class Logging
{
public:
	static void SetLogVerbosity(ELogVerbosity Verbosity);
	static ELogVerbosity GetLogVerbosity();

	static void LogVerbose(const std::string& DebugScope, const std::string& DebugString);
	static void Log(const std::string& DebugScope, const std::string& DebugString);
	static void LogWarning(const std::string& DebugScope, const std::string& DebugString);
	static void LogError(const std::string& DebugScope, const std::string& DebugString);

	static void LogVerbose(const std::wstring& DebugScope, const std::wstring& DebugString);
	static void Log(const std::wstring& DebugScope, const std::wstring& DebugString);
	static void LogWarning(const std::wstring& DebugScope, const std::wstring& DebugString);
	static void LogError(const std::wstring& DebugScope, const std::wstring& DebugString);

protected:
	static ELogVerbosity LogVerbosity;
	static void SetLogColor(uint8_t Color);
	static uint8_t LogColor;
};

#ifdef LOGGING

#define LOG(DebugScope, DebugString) Logging::Log(DebugScope, DebugString);
#define LOGVERBOSE(DebugScope, DebugString) Logging::LogVerbose(DebugScope, DebugString);
#define LOGWARNING(DebugScope, DebugString) Logging::LogWarning(DebugScope, DebugString);
#define LOGERROR(DebugScope, DebugString) Logging::LogError(DebugScope, DebugString);

#else

#define LOG(DebugScope, DebugString)
#define LOGVERBOSE(DebugScope, DebugString)
#define LOGWARNING(DebugScope, DebugString)
#define LOGERROR(DebugScope, DebugString)

#endif