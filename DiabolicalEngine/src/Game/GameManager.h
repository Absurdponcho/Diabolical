#pragma once
#include "ECS/ECS.h"
#include "Types/DMemory.h"

class DGameManager
{
public:
	DGameManager() {};

	void RenderingTest();

	// called by the engine to start the game. don't call this manually
	void MainGameLoop();

	// Get frames per second
	float GetGameFPS();

	// Time since last frame
	float GetDeltaTime();

	// Total elapsed time of the game
	float GetGameTime();

	// The current frame
	int GetFrame();

	// Exits the game at the end of the current frame
	void Exit();

	static DGameManager& Get();

	flecs::world& GetECSWorld();

private:
	void GameTick();
	flecs::world ECSWorld;

	bool bMainLoopRunning = true;

	int Frame = 0;
	float GameTime = 0;
};