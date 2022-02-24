#pragma once
#include "../ImGui/imgui.h"
#include <SDL2/SDL.h>
#include "../Graphics/WindowManager.h"
#include "../ImGui/backends/imgui_impl_opengl3.h"
#include "../ImGui/backends/imgui_impl_sdl.h"
#include "../GUI/IWindow.h"

// Temporary, moving these here to clean up GameManager
void InitImGui();
void ImGuiTick();