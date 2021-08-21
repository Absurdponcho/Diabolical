#include "WindowManager.h"
#include "Logging/Logging.h"

WindowManager::WindowManager(const char* title, int x, int y, int w, int h, Uint32 flags) :
	x(x), y(y), w(y), h(h), WindowFlags(flags), bWindowValid(true)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		Logging::LogError("WindowManager::WindowManager()", "SDL_Init() failed!");
		bWindowValid = false;
	}
	Logging::Log("WindowManager::WindowManager()", "SDL_Init() success");

	GameWindow = SDL_CreateWindow(title, x, y, w, h, flags | SDL_WINDOW_RESIZABLE);
	GameSurface = SDL_GetWindowSurface(GameWindow);
	if (GameSurface == NULL) {
		Logging::LogError("WindowManager::WindowManager()", "SDL_GetWindowSurface() failed");
		bWindowValid = false;
	}
	GameRenderer = SDL_CreateRenderer(GameWindow, -1, SDL_RENDERER_ACCELERATED);
	if (GameRenderer == NULL) {
		Logging::LogError("WindowManager::WindowManager()", "SDL_CreateRenderer() failed");
		bWindowValid = false;
	}
}

WindowManager::~WindowManager()
{
	SDL_DestroyRenderer(GameRenderer);
	SDL_DestroyWindow(GameWindow);
	SDL_Quit();
}

SDL_Window* WindowManager::GetSDLWindow()
{
	return GameWindow;
}

SDL_Surface* WindowManager::GetSDLSurface()
{
	return GameSurface;
}

SDL_Renderer* WindowManager::GetSDLRenderer()
{
	return GameRenderer;
}

bool WindowManager::IsValid()
{
	return bWindowValid;
}
