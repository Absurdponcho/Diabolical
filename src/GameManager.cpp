#include "GameManager.h"
#include "ECS/GameComponent.h"
#include "Logging/Logging.h"

void GameManager::MainGameLoop()
{
	Logging::LogVerbose("GameManager::MainGameLoop()", "Main game loop started");
	while (bMainLoopRunning)
	{
		ManagerTick();
	}
}

void GameManager::ManagerTick()
{
	GameBaseObject::SpawnPendingObjects();
	GameBaseObject::DestroyPendingObjects();
	GameBaseObject::TickAllObjects();
}