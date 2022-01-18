#pragma once
#include <string>
#include "../Types/DString.h"

#define LOG(x) Logging::LogString("LOG", __FUNCTION__, __LINE__, x, 15)
#define LOG_WARN(x) Logging::LogString("WARNING", __FUNCTION__, __LINE__, x, 14)
#define LOG_ERR(x) Logging::LogString("ERROR", __FUNCTION__, __LINE__, x, 12)

namespace Logging
{
	void LogString(DString Prefix, DString Func, int Line, DString String, int Color);
}