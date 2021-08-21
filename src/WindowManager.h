#pragma once
#include <Windows.h>
#include <string>
#include <SDL2/SDL.h>
static SDL_Window* GameWindow;
static SDL_Surface* GameSurface;
static SDL_Renderer* GameRenderer;
class WindowManager
{
public:
	WindowManager(const char* WindowTitle, int x, int y, int w, int h, Uint32 WindowFlags);
	~WindowManager();
	static SDL_Window* GetSDLWindow();
	static SDL_Surface* GetSDLSurface();
	static SDL_Renderer* GetSDLRenderer();
	bool IsValid();

private:

	int x, y, w, h;
	Uint32 WindowFlags;
	bool bWindowValid;

};
