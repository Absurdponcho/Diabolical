#include "GameManager.h"
#include "CoreEngine.h"
#include <chrono>
#include "Audio/AudioAsset.h"
#include "Rendering/Camera.h"

float GameManager::FPS = 0;
float GameManager::GameTime = 0;

void GameManager::MainGameLoop()
{
	Logging::LogVerbose("GameManager::MainGameLoop()", "Main game loop started");
	while (bMainLoopRunning)
	{
		EventTick();
		ManagerTick();
	}
}

int GameManager::GetGameFPS()
{
	return (int)FPS;
}

void GameManager::EventTick()
{
	SDL_Event Event;
	int bEvent = SDL_PollEvent(&Event);
	if (bEvent && Event.type == SDL_QUIT)
		bMainLoopRunning = false;
}

void GameManager::ManagerTick()
{

	static long long OldTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	long long Time = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	long long TimeDiff = Time - OldTime;
	OldTime = Time;

	float DeltaTime = (float)TimeDiff / 1000000000.f;

	FPS = 1.f / DeltaTime;

	GameBaseObject::SpawnPendingObjects();
	GameBaseObject::DestroyPendingObjects();
	PhysicsWorld::Get().Step();
	GameBaseObject::TickAllObjects(DeltaTime);

	SDL_Renderer* renderer = WindowManager::GetSDLRenderer();
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	if (CameraComponent* Camera = CameraComponent::GetActiveCamera())
	{
		Camera->Draw();
	}

	SDL_RenderPresent(renderer);

	GameTime += DeltaTime;
}

float GameManager::GetFPS() 
{
	return FPS;
}

float GameManager::GetTime() 
{
	return GameTime;
}