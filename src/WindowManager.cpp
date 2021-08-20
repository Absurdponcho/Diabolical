#include "WindowManager.h"

WindowManager::WindowManager(const char* title, int x, int y, int w, int h, Uint32 flags)
{

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		// printboi error
	}
	GameWindow = SDL_CreateWindow(title, x, y, w, h, flags);
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->WindowFlags = flags;
}