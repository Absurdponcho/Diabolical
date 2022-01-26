#include "DiabolicalEngine.h"
#include "Graphics/WindowManager.h"
#include "Game/GameManager.h"
#include "Logging/Logging.h"
#include "CommandLine/CmdLine.h"
#include "Check.h"
#include "Networking/Socket.h" 
#include "Thread/Thread.h"
#include "Thread/GameThread.h"

#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#include <Dbghelp.h>
#endif

DGameManager* DEngine::GameManager = nullptr;

void DEngine::Init(int argc, char* argv[])
{
#ifdef PLATFORM_WINDOWS
    SetUnhandledExceptionFilter(MinidumpExceptionFilter);
#endif
	DSocket::InitSockets();

	atexit(AtExit);
	DCommandLine::Init(argc, argv);

	GameManager = new DGameManager();
	DWindowManager::Initialize("Diabolical", 128, 128, 800, 600, SDL_WINDOW_HIDDEN);

	RunAllTests();
}

void DEngine::Run()
{
	Check(GameManager);
	GameManager->MainGameLoop();
}

void DEngine::AtExit()
{
	Logging::CloseLogFile();
}

void DEngine::RunAllTests()
{
	LOG("==================== Testing Threads ====================");
	DThread::TestThreadFunctions();
	LOG("==================== Threads Complete ===================");

	LOG("==================== Testing Sockets ====================");
	DSocket::NetworkTest();
	LOG("==================== Sockets Complete ===================");
}

#ifdef PLATFORM_WINDOWS
long MinidumpExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo)
{
    MessageBox(NULL, "Error! Unhandled Exception!", "SussyError", 0);
	auto hDbgHelp = LoadLibraryA("dbghelp");
	if (hDbgHelp == nullptr)
		return EXCEPTION_CONTINUE_SEARCH;
	auto pMiniDumpWriteDump = (decltype(&MiniDumpWriteDump))GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
	if (pMiniDumpWriteDump == nullptr)
		return EXCEPTION_CONTINUE_SEARCH;

    char name[MAX_PATH];
    {
        auto nameEnd = name + GetModuleFileNameA(GetModuleHandleA(0), name, MAX_PATH);
        SYSTEMTIME t;
        GetSystemTime(&t);
        wsprintfA(nameEnd - strlen(".exe"),
            "_%4d%02d%02d_%02d%02d%02d.dmp",
            t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
    }

    auto hFile = CreateFileA(name, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (hFile == INVALID_HANDLE_VALUE)
        return EXCEPTION_CONTINUE_SEARCH;

    MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;
    exceptionInfo.ThreadId = GetCurrentThreadId();
    exceptionInfo.ExceptionPointers = ExceptionInfo;
    exceptionInfo.ClientPointers = FALSE;

    auto dumped = pMiniDumpWriteDump(
        GetCurrentProcess(),
        GetCurrentProcessId(),
        hFile,
        MINIDUMP_TYPE(MiniDumpWithIndirectlyReferencedMemory | MiniDumpScanMemory),
        ExceptionInfo ? &exceptionInfo : nullptr,
        nullptr,
        nullptr);

    CloseHandle(hFile);

    return EXCEPTION_CONTINUE_SEARCH;
}
#endif