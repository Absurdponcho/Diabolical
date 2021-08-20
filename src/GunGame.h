#pragma once
#include <cassert>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define PLATFORM_WINDOWS
#endif

#define Check(Condition) assert(Condition)