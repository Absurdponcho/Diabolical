#include "GameManager.h"
#include <chrono>
#include <SDL2/SDL.h>
#include "../Graphics/WindowManager.h"
#include <gl/glew.h>
#include <gl/GL.h>
#include "../Audio/AudioSource.h"
#include "../Audio/WAVFile.h"

DAudioSource* AudioSource;
void DGameManager::MainGameLoop()
{
	//LOGVERBOSE("GameManager::MainGameLoop()", "Main game loop started");

	AudioSource = new DAudioSource();
	{
		std::shared_ptr<DWAVFile> WavFile = DWAVFile::Load("Assets/Sussy Baka.wav");
		AudioSource->SetAudioFile(WavFile);
		AudioSource->Play();
	}

	// do the first tick before showing the window to prevent icky sticky white window
	EventTick();
	SDL_ShowWindow(DWindowManager::GetSDLWindow());
	SDL_GL_SetSwapInterval(-1);
	while (bMainLoopRunning)
	{
		EventTick();
		SDL_GL_SwapWindow(DWindowManager::GetSDLWindow());
	}
}

int DGameManager::GetGameFPS()
{
	return (int)0;
}

void DGameManager::EventTick()
{
	SDL_Event Event;
	while (SDL_PollEvent(&Event)) {

		switch (Event.type) {

		case SDL_KEYDOWN:
			break;
		case SDL_KEYUP:
			//if (Event.key.keysym.sym )
			//InputManager::HandleKeyboardEvent(Event.key);
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			//InputManager::HandleMouseButtonEvent(Event.button);
			break;
		case SDL_MOUSEWHEEL:
			//InputManager::HandleMouseWheelEvent(Event.wheel);
			break;

		case SDL_QUIT:
			bMainLoopRunning = false;
			break;

		case SDL_WINDOWEVENT: // screen resize thing
			if (Event.window.event != SDL_WINDOWEVENT_SIZE_CHANGED) break;
			
			glViewport(0, 0, DWindowManager::Get().GetScreenSize().x, DWindowManager::Get().GetScreenSize().y);
			break;

		default:
			break;
		}
	}
}

