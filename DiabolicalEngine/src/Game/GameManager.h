#pragma once
#include "ECS/ECS.h"
#include "Types/DMemory.h"
#include "Meta/Meta.h"
#include "Maths/Maths.h"

class Test1 : public DMetaBase
{
	GENERATE_CLASS_META(Test1)
public:
	Test1() {}

	PROPERTYDEF(int, TestInt, 69)

	int NormalInt = 3;
	float Float = 69.3f;

	PROPERTY(Vector3, TestVector3)
	PROPERTY(Vector3*, TestVectorPtr)
};


class Test2 : public Test1
{
	GENERATE_CLASS_META(Test2);
public:
	Test2() {}

	PROPERTY(Vector3, asdas)
};

class Test3 : public Test2
{
	GENERATE_CLASS_META(Test3);
public:
	Test3() {}
};

class Test4 : public Test3
{
	GENERATE_CLASS_META(Test4);
public:
	Test4() {}

	PROPERTY(Matrix4x4, asdas123asd)
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