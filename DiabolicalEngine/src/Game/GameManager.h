#pragma once
#include "ECS/flecs.h"

class DGameManager
{
public:
	DGameManager();

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

private:
	void GameTick();
	flecs::world ECSWorld;

	bool bMainLoopRunning = true;

	int Frame = 0;
	float GameTime = 0;
};