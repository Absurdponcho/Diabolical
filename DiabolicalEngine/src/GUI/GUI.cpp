#include "GUI.h"

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