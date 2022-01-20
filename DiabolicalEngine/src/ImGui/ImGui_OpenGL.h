#pragma once
#define IMGUI_IMPL_OPENGL_ES3

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl3_loader.h"
#include <SDL2/SDL.h>
struct ImGui_OpenGL
{
	static ImGuiIO& io;
	static SDL_GLContext gl_context;
	static bool SetupBackend(SDL_Window* window);
	static bool ShutdownBackend();
	static void CreateFrame();
	static void RenderFrame();
};

