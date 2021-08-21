#pragma once

class GameManager
{
public:
	void MainGameLoop();

	int GetGameFPS();

	static float GetFPS();
	static float GetTime();

private:
	void EventTick();
	void ManagerTick();

	bool bMainLoopRunning = true;

	static float FPS;
	static float GameTime;
};