#include "DiabolicalEngine.h"
#include "Graphics/WindowManager.h"
#include "Game/GameManager.h"
#include "Logging/Logging.h"
#include "CommandLine/CmdLine.h"
#include "Check.h"
#include "Networking/Socket.h"

DGameManager* DEngine::GameManager = nullptr;

void DEngine::Init(int argc, char* argv[])
{
	DSocket::InitSockets();

	atexit(AtExit);
	DCommandLine::Init(argc, argv);

	GameManager = new DGameManager();
	DWindowManager::Initialize("Diabolical", 128, 128, 800, 600, SDL_WINDOW_HIDDEN);

	DSocket::NetworkTest();
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
