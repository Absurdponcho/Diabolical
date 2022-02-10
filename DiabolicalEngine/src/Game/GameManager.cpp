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
#include "../GUI/DevConsole/DevConsole.h"

std::shared_ptr<DDevConsole> DevConsole;

void InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::GetIO().KeyMap[ImGuiKey_Backspace] = SDL_GetScancodeFromKey(SDLK_BACKSPACE);
	ImGui::GetIO().KeyMap[ImGuiKey_Delete] = SDL_GetScancodeFromKey(SDLK_DELETE);
	ImGui::GetIO().KeyMap[ImGuiKey_Tab] = SDL_GetScancodeFromKey(SDLK_TAB);
	ImGui::GetIO().KeyMap[ImGuiKey_LeftArrow] = SDL_GetScancodeFromKey(SDLK_LEFT);
	ImGui::GetIO().KeyMap[ImGuiKey_UpArrow] = SDL_GetScancodeFromKey(SDLK_UP);
	ImGui::GetIO().KeyMap[ImGuiKey_RightArrow] = SDL_GetScancodeFromKey(SDLK_RIGHT);
	ImGui::GetIO().KeyMap[ImGuiKey_DownArrow] = SDL_GetScancodeFromKey(SDLK_DOWN);
	ImGui::GetIO().KeyMap[ImGuiKey_::ImGuiKey_Home] = SDL_GetScancodeFromKey(SDLK_HOME);
	ImGui::GetIO().KeyMap[ImGuiKey_::ImGuiKey_Escape] = SDL_GetScancodeFromKey(SDLK_ESCAPE);
	ImGui::GetIO().KeyMap[ImGuiKey_::ImGuiKey_End] = SDL_GetScancodeFromKey(SDLK_END);
	ImGui::GetIO().KeyMap[ImGuiKey_::ImGuiKey_PageUp] = SDL_GetScancodeFromKey(SDLK_PAGEUP);
	ImGui::GetIO().KeyMap[ImGuiKey_::ImGuiKey_PageDown] = SDL_GetScancodeFromKey(SDLK_PAGEDOWN);
	ImGui::GetIO().KeyMap[ImGuiKey_::ImGuiKey_Insert] = SDL_GetScancodeFromKey(SDLK_INSERT);
	ImGui::GetIO().KeyMap[ImGuiKey_::ImGuiKey_KeyPadEnter] = SDL_GetScancodeFromKey(SDLK_KP_ENTER);
	ImGui::GetIO().KeyMap[ImGuiKey_::ImGuiKey_Enter] = SDL_GetScancodeFromKey(SDLK_RETURN);

	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui_ImplSDL2_InitForOpenGL(DWindowManager::GetSDLWindow(), DWindowManager::GetGLContext());
	
	static TestWindow testWindow = TestWindow();
	testWindow.Enable();

	DevConsole = std::make_shared<DDevConsole>();

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

		DThread::CheckManagedThreads();
		DGameThread::RunInvokedFunctions();
	}
}

int DGameManager::GetGameFPS()
{
	return (int)0;
}

void HandleSDLDown(SDL_Event& Event)
{
	if (ImGui::GetIO().WantTextInput)
	{
		ImGui::GetIO().KeysDown[Event.key.keysym.scancode] = true;

		if (Event.key.keysym.scancode == SDL_SCANCODE_LSHIFT ||
			Event.key.keysym.scancode == SDL_SCANCODE_RSHIFT)
		{
			ImGui::GetIO().KeyShift = true;
		}

		if (Event.key.keysym.scancode == SDL_SCANCODE_LCTRL ||
			Event.key.keysym.scancode == SDL_SCANCODE_RCTRL)
		{
			ImGui::GetIO().KeyCtrl = true;
		}

		if (Event.key.keysym.scancode == SDL_SCANCODE_LALT ||
			Event.key.keysym.scancode == SDL_SCANCODE_RALT)
		{
			ImGui::GetIO().KeyAlt = true;
		}
	}
	else
	{
		if (Event.key.keysym.scancode == SDL_SCANCODE_GRAVE)
		{
			if (DevConsole.get())
			{
				DevConsole->Toggle();
			}
		}
	}
}

void HandleSDLUp(SDL_Event& Event)
{
	ImGui::GetIO().KeysDown[Event.key.keysym.scancode] = false;

	if (Event.key.keysym.scancode == SDL_SCANCODE_LSHIFT ||
		Event.key.keysym.scancode == SDL_SCANCODE_RSHIFT)
	{
		ImGui::GetIO().KeyShift = false;
	}

	if (Event.key.keysym.scancode == SDL_SCANCODE_LCTRL ||
		Event.key.keysym.scancode == SDL_SCANCODE_RCTRL)
	{
		ImGui::GetIO().KeyCtrl = false;
	}

	if (Event.key.keysym.scancode == SDL_SCANCODE_LALT ||
		Event.key.keysym.scancode == SDL_SCANCODE_RALT)
	{
		ImGui::GetIO().KeyAlt = false;
	}
}

void DGameManager::EventTick()
{
	SDL_Event Event;
	while (SDL_PollEvent(&Event)) {

		switch (Event.type) {

		case SDL_KEYDOWN:
			HandleSDLDown(Event);
			break;
		case SDL_KEYUP:
			HandleSDLUp(Event);
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			break;
		case SDL_MOUSEWHEEL:
			break;
		case SDL_QUIT:
			bMainLoopRunning = false;
			break;
		case SDL_WINDOWEVENT: // screen resize thing
			if (Event.window.event != SDL_WINDOWEVENT_SIZE_CHANGED) break;
			glViewport(0, 0, DWindowManager::Get().GetScreenSize().x, DWindowManager::Get().GetScreenSize().y);
			break;
		case SDL_TEXTINPUT:
			if (ImGui::GetIO().WantTextInput)
			{
				ImGui::GetIO().AddInputCharactersUTF8(Event.text.text);
			}
		default:
			break;
		}
	}
}

