#pragma once
#include <string>

// Example 
// Logging::Log("Class::Function", "Debug String");

enum class ELogVerbosity : uint8_t
{
	LV_ErrorOnly = 0,
	LV_WarningOnly = 1,
	LV_All = 2,
};

class Logging
{
public:
	static void SetLogVerbosity(ELogVerbosity Verbosity);
	static ELogVerbosity GetLogVerbosity();

	static void Log(const std::string& DebugScope, const std::string& DebugString);
	static void LogWarning(const std::string& DebugScope, const std::string& DebugString);
	static void LogError(const std::string& DebugScope, const std::string& DebugString);

protected:
	static ELogVerbosity LogVerbosity;
	static void SetLogColor(uint8_t Color);
	static uint8_t LogColor;
};