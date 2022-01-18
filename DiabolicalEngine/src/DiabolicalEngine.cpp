#include "DiabolicalEngine.h"
#include "Graphics/WindowManager.h"
#include "Game/GameManager.h"
#include "Logging/Logging.h"
#include "CommandLine/CmdLine.h"
#include "Check.h"

DGameManager* DEngine::GameManager = nullptr;

void DEngine::Init(int argc, char* argv[])
{
	atexit(AtExit);
	DCommandLine::Init(argc, argv);

	GameManager = new DGameManager();
	DWindowManager::Initialize("Diabolical", 128, 128, 800, 600, SDL_WINDOW_HIDDEN);
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
