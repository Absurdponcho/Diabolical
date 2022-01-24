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

void InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui_ImplSDL2_InitForOpenGL(DWindowManager::GetSDLWindow(), DWindowManager::GetGLContext());
	
	static TestWindow testWindow = TestWindow();
	testWindow.Enable();

	ImGui::StyleColorsDark();

}

void ImGuiTick()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	for (IWindow* window : IWindow::WindowList) {
		window->Tick();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

DAudioSource* AudioSource;
void DGameManager::MainGameLoop()
{
	//LOGVERBOSE("GameManager::MainGameLoop()", "Main game loop started");

	AudioSource = new DAudioSource();
	{
		std::shared_ptr<DWAVFile> WavFile = DWAVFile::Load("Assets/Sussy Baka.wav"); // Hey dude, you're being quite sussy
		AudioSource->SetAudioFile(WavFile);
		AudioSource->Play();
	}

	InitImGui();

	// do the first tick before showing the window to prevent icky sticky white window
	EventTick();
	SDL_ShowWindow(DWindowManager::GetSDLWindow());
	SDL_GL_SetSwapInterval(-1);
	while (bMainLoopRunning)
	{
		EventTick();

		glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGuiTick();

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

