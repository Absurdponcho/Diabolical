#include "GameManager.h"
#include "CoreEngine.h"
#include <chrono>
#include "Audio/AudioAsset.h"
#include "Rendering/Camera.h"
#include "Textures/TextureAsset.h"
#include <SDL2/SDL.h>
#include "Input/InputManager.h"

float GameManager::FPS = 0;
float GameManager::GameTime = 0;

void GameManager::MainGameLoop()
{
	Logging::LogVerbose("GameManager::MainGameLoop()", "Main game loop started");

	// do the first tick before showing the window to prevent icky sticky white window
	EventTick();
	ManagerTick();
	SDL_ShowWindow(WindowManager::GetSDLWindow());

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
	while (SDL_PollEvent(&Event)) {
		switch (Event.type) {
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			InputManager::HandleKeyboardEvent(Event.key);
			break;

		case SDL_QUIT:
			bMainLoopRunning = false;
			break;

		default:
			break;
		}
	}
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

	GameBaseObject::PostRenderAllObjects(DeltaTime);

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