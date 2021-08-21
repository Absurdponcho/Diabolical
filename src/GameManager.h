#pragma once

class GameManager
{
public:
	void MainGameLoop();

	int GetGameFPS();

private:
	void ManagerTick();

	bool bMainLoopRunning = true;

	float FPS = 0;
};