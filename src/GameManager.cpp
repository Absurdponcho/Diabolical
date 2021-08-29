#include "GameManager.h"
#include "CoreEngine.h"
#include "opengl.h"
#include <chrono>
#include "Audio/AudioAsset.h"
#include "Rendering/Camera.h"
#include "Textures/TextureAsset.h"
#include <SDL2/SDL.h>
#include "Input/InputManager.h"
#include "WindowManager.h"
#include "Rendering/Framebuffer.h"
#include "Rendering/RenderPipeline.h"

float GameManager::FPS = 0;
float GameManager::GameTime = 0;
int GameManager::Frame = 0;


void GameManager::MainGameLoop()
{
	Logging::LogVerbose("GameManager::MainGameLoop()", "Main game loop started");

	// do the first tick before showing the window to prevent icky sticky white window
	EventTick();
	ManagerTick();
	SDL_ShowWindow(WindowManager::GetSDLWindow());
	SDL_GL_SetSwapInterval(-1);
	while (bMainLoopRunning)
	{
		EventTick();
		ManagerTick();
	}
}

int GameManager::GetGameFPS()
{
	return (int)FPS;
}

void GameManager::EventTick()
{
	SDL_Event Event;
	while (SDL_PollEvent(&Event)) {

		switch (Event.type) {

		case SDL_KEYDOWN:
		case SDL_KEYUP:
			InputManager::HandleKeyboardEvent(Event.key);
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			InputManager::HandleMouseButtonEvent(Event.button);
			break;
		case SDL_MOUSEWHEEL:
			InputManager::HandleMouseWheelEvent(Event.wheel);
			break;

		case SDL_QUIT:
			bMainLoopRunning = false;
			break;

		case SDL_WINDOWEVENT: // screen resize thing
			if (Event.window.event != SDL_WINDOWEVENT_SIZE_CHANGED) break;
			//Logging::Log("", std::to_string(Event.window.event));
			glViewport(0, 0, WindowManager::Get().GetScreenSize().x, WindowManager::Get().GetScreenSize().y);
			if (RenderPipeline* RP = RenderPipeline::GetActive())
			{
				RP->Bind(); // rebind the render pipeline so we can regen the framebuffers
			}
			break;

		default:
			break;
		}
	}
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
	PhysicsWorld::Get().Step();
	GameBaseObject::TickAllObjects(DeltaTime);

	if (RenderPipeline* Pipeline = RenderPipeline::GetActive())
	{
		Pipeline->Draw(CameraComponent::GetActiveCamera(), DeltaTime);
	}

	GameBaseObject::PostRenderAllObjects(DeltaTime);


	GameTime += DeltaTime;
	Frame++;
}

int GameManager::GetFrame()
{
	return Frame;
}

float GameManager::GetFPS() 
{
	return FPS;
}

float GameManager::GetTime() 
{
	return GameTime;
}