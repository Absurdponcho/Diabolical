#include "GameManager.h"
#include "ECS/GameComponent.h"
#include "Logging/Logging.h"
#include <chrono>
#include "Rendering/GameRendererComponent.h"

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

	GameRendererComponent::CullAllRenderers(RendererStack);
	GameRendererComponent::RenderAllRenderers(RendererStack);
}