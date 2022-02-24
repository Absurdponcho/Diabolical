#include "GameManager.h"
#include <chrono>
#include <SDL2/SDL.h>
#include "../Graphics/WindowManager.h"
#include <gl/glew.h>
#include <gl/GL.h>
#include "../Audio/AudioSource.h"
#include "../Audio/WAVFile.h"
#include "../ImGui/imgui.h"
#include "../ImGui/backends/imgui_impl_opengl3.h"
#include "../ImGui/backends/imgui_impl_sdl.h"
#include "../GUI/IWindow.h"
#include "../GUI/TestWindow.h"
#include "../Thread/GameThread.h"
#include "../Thread/Thread.h"
#include "../AssetManager/AssetManager.h"
#include "../ECS/flecs.h"
#include "../GUI/GUI.h"
#include "../Input/Input.h"

DGameManager::DGameManager()
{

}

void DGameManager::Exit()
{
	bMainLoopRunning = false;
}

void DGameManager::MainGameLoop()
{
	glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT); 
	EventTick();
	SDL_ShowWindow(DWindowManager::GetSDLWindow());

	while (bMainLoopRunning)
	{
		EventTick();
		GameTick();

		glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGuiTick();
		SDL_GL_SwapWindow(DWindowManager::GetSDLWindow());

		DThread::CheckManagedThreads();
		DGameThread::RunInvokedFunctions();

		Frame++;
		GameTime += ECSWorld.delta_time();
	}
}

void DGameManager::GameTick()
{
	ECSWorld.progress();
}

float DGameManager::GetDeltaTime()
{
	return ECSWorld.delta_time();
}

float DGameManager::GetGameFPS()
{
	return 1/ECSWorld.delta_time();
}

float DGameManager::GetGameTime()
{
	return GameTime;
}

int DGameManager::GetFrame()
{
	return Frame;
}