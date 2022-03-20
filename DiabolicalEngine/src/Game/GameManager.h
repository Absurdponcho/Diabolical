#pragma once
#include "ECS/ECS.h"
#include "Types/DMemory.h"
#include "Meta/Meta.h"
#include "Maths/Maths.h"

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


private:
	void GameTick();

	bool bMainLoopRunning = true;

	int Frame = 0;
	float GameTime = 0;
};