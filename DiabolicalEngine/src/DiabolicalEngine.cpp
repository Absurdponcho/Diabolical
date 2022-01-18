#include "DiabolicalEngine.h"
#include "Graphics/WindowManager.h"
#include "Game/GameManager.h"

DGameManager* DEngine::GameManager = nullptr;

void DEngine::Init(int argc, char* argv[])
{
	GameManager = new DGameManager();
	DWindowManager::Initialize("Diabolical", 128, 128, 800, 600, SDL_WINDOW_HIDDEN);
}

void DEngine::Run()
{
	assert(GameManager);
	GameManager->MainGameLoop();
}