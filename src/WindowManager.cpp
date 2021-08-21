#include "WindowManager.h"
#include "Logging/Logging.h"

WindowManager::WindowManager(const char* title, int x, int y, int w, int h, Uint32 flags) :
	x(x), y(y), w(w), h(h), WindowFlags(flags), bWindowValid(true)
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

b2Vec2 WindowManager::PixelCoordToScreenSpace(const b2Vec2& PixelSpace)
{
	b2Vec2 ScreenSpace;
	ScreenSpace.x = ((PixelSpace.x / w) - .5f) * 2;
	ScreenSpace.y = ((PixelSpace.y / h) - .5f) * 2;
	return ScreenSpace;
}

b2Vec2 WindowManager::ScreenSpaceToPixelCoord(const b2Vec2& ScreenSpace)
{
	b2Vec2 PixelSpace;
	PixelSpace.x = (ScreenSpace.x + 1) * w / 2;
	PixelSpace.y = (ScreenSpace.y + 1) * h / 2;
	return PixelSpace;
}