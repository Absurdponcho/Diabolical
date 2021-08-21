#pragma once
#include <Windows.h>
#include <string>
#include <SDL2/SDL.h>

class WindowManager
{
public:
	WindowManager(const char* WindowTitle, int x, int y, int w, int h, Uint32 WindowFlags);
	SDL_Window* GetSDLWindow();
	SDL_Surface* GetSDLSurface();
	SDL_Renderer* GetSDLRenderer();
	bool IsValid();

private:
	SDL_Window* GameWindow;
	SDL_Surface* GameSurface;
	SDL_Renderer* GameRenderer;
	int x, y, w, h;
	Uint32 WindowFlags;
	bool bWindowValid;

};
