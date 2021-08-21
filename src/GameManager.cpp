#include "GameManager.h"
#include "CoreEngine.h"
#include <chrono>
#include "Audio/AudioAsset.h"

void GameManager::MainGameLoop()
{
	Logging::LogVerbose("GameManager::MainGameLoop()", "Main game loop started");
	while (bMainLoopRunning)
	{
		ManagerTick();
	}
}

int GameManager::GetGameFPS()
{
	return (int)FPS;
}

void GameManager::ManagerTick()
{

	SDL_Event Event;
	if (SDL_PollEvent(&Event) && Event.type == SDL_QUIT)
		bMainLoopRunning = false;

	static long long OldTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	long long Time = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	long long TimeDiff = Time - OldTime;
	OldTime = Time;

	float DeltaTime = (float)TimeDiff / 1000000000.f;

	FPS = 1.f / DeltaTime;

	GameBaseObject::SpawnPendingObjects();
	GameBaseObject::DestroyPendingObjects();
	GameBaseObject::TickAllObjects(DeltaTime);

	std::stack<GameRendererComponent*> RendererStack;

	SDL_Renderer* renderer = WindowManager::GetSDLRenderer();
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	GameRendererComponent::CullAllRenderers(RendererStack);
	GameRendererComponent::RenderAllRenderers(RendererStack);

	SDL_RenderPresent(renderer);

}