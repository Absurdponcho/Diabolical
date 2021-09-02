#pragma once
#include <cassert>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define PLATFORM_WINDOWS
#endif

#define Check(Condition) assert(Condition)

#define Out


// Initialize all the engine things
void InitializeEngine(int argc, char** argv);

// This will block until the game window is closed
void RunEngineLoop();