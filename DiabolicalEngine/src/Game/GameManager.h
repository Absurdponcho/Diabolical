#pragma once

class DGameManager
{
public:
	void MainGameLoop();

	int GetGameFPS();

	static float GetFPS();
	static float GetTime();
	static int GetFrame();

private:
	void EventTick();

	bool bMainLoopRunning = true;

	static int Frame;
	static float FPS;
	static float GameTime;
};