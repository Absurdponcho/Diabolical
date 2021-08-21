#include "GameManager.h"
#include "CoreEngine.h"
#include <chrono>

void GameManager::MainGameLoop()
{
	Logging::LogVerbose("GameManager::MainGameLoop()", "Main game loop started");
	while (bMainLoopRunning)
	{
		ManagerTick();
	}
}

int GameManager::GetGameFPS()
{
	return (int)FPS;
}

void GameManager::ManagerTick()
{
	static long long OldTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	long long Time = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	long long TimeDiff = Time - OldTime;
	OldTime = Time;

	float DeltaTime = (float)TimeDiff / 1000000000.f;

	FPS = 1.f / DeltaTime;

	GameBaseObject::SpawnPendingObjects();
	GameBaseObject::DestroyPendingObjects();
	GameBaseObject::TickAllObjects(DeltaTime);

	std::stack<GameRendererComponent*> RendererStack;

	GameRendererComponent::CullAllRenderers(RendererStack);
	GameRendererComponent::RenderAllRenderers(RendererStack);

	static float Timer = 0.3f;
	Timer -= DeltaTime;
	if (Timer <= 0)
	{
		Timer = 0.5f;
		GameAssetSoftPointer AudioAsset("GameAssetFiles/phaser1.wav");
		GameAudio::PlaySound(AudioAsset, 0.2f);
	}
}