#pragma once
#include <Windows.h>
#include <string>
#include <SDL2/SDL.h>

class WindowManager
{
public:
	WindowManager(const char* WindowTitle, int x, int y, int w, int h, Uint32 WindowFlags);
	SDL_Window* GameWindow;
	std::string WindowTitle;
	int x, y, w, h;
	Uint32 WindowFlags;
};
