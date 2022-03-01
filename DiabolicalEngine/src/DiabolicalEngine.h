#pragma once
#include "ECS/ECS.h"
#include <iostream>
#include "CommandLine/CmdLine.h"
#include "Logging/Logging.h"
#include "Types/DMemory.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

#define PLATFORM_WINDOWS
long MinidumpExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo);
#endif


class DEngine
{
public:
	static void Init(int argc, char *argv[]);
	static void Run();
	static void AtExit();
	static void RunAllTests();
	static DSharedPtr<class DDevConsole> DevConsole;
	static class DGameManager* GameManager;

private:
};

