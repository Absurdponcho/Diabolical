#pragma once

class GameManager
{
public:
	void MainGameLoop();
private:
	void ManagerTick();

	bool bMainLoopRunning = true;
};