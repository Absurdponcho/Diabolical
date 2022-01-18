#pragma once
#include "ECS/flecs.h"
#include <iostream>
#include "CommandLine/CmdLine.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define PLATFORM_WINDOWS
#endif

class DEngine
{
public:
	static void Init(int argc, char *argv[]);
	static void Run();

private:
	static class DGameManager* GameManager;
};