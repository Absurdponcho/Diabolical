#pragma once
#include <SDL2/SDL.h>
#include "ImGui/imgui.h"
#include "DiabolicalEngine.h"
#include "Game/GameManager.h"
#include "Graphics/WindowManager.h"
#include "GUI/DevConsole/DevConsole.h"

// temporary, putting these here to clean up GameManager

void HandleSDLDown(SDL_Event& Event);
void HandleSDLUp(SDL_Event& Event);
void EventTick();