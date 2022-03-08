#pragma once
#include "ECS/ECS.h"
#include "Types/DMemory.h"
#include "Types/Meta.h"
#include "Maths/Maths.h"

class TestReflectedClass
{
	GENERATE_CLASS_META(TestReflectedClass);
public:
	TestReflectedClass() {}

	PROPERTY(int, TestInt)
	PROPERTY(int, TestInt2)
	PROPERTY(int, TestInt3)

	int NormalInt = 3;

	float Float = 69.3f;

	PROPERTY(Vector3, TestVector3)

};

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