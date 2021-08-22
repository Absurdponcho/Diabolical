#include "WindowManager.h"
#include "Logging/Logging.h"
#include "GunGame.h"

WindowManager* WindowManager::Singleton;

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

void WindowManager::Initialize(const char* WindowTitle, int x, int y, int w, int h, Uint32 WindowFlags)
{
	static bool bInitialized = false;

	Check(!bInitialized);
	bInitialized = true;
	Singleton = new WindowManager(WindowTitle, x, y, w, h, WindowFlags);
}

WindowManager::~WindowManager()
{
	SDL_DestroyRenderer(GameRenderer);
	SDL_DestroyWindow(GameWindow);
	SDL_Quit();
}

SDL_Window* WindowManager::GetSDLWindow()
{
	Check(GameWindow);
	return GameWindow;
}

SDL_Surface* WindowManager::GetSDLSurface()
{
	Check(GameSurface);

	return GameSurface;
}

SDL_Renderer* WindowManager::GetSDLRenderer()
{
	Check(GameRenderer);

	return GameRenderer;
}

bool WindowManager::IsValid()
{
	return bWindowValid;
}

glm::vec2 WindowManager::PixelCoordToScreenSpace2D(const glm::vec2& PixelSpace) const
{
	glm::vec2 ScreenSpace;
	ScreenSpace.x = ((PixelSpace.x / w) - .5f) * 2;
	ScreenSpace.y = ((PixelSpace.y / h) - .5f) * 2;
	return ScreenSpace;
}

glm::vec2 WindowManager::ScreenSpaceToPixelCoord2D(const glm::vec2& ScreenSpace) const
{
	glm::vec2 PixelSpace;
	PixelSpace.x = (ScreenSpace.x + 1) * w / 2;
	PixelSpace.y = (ScreenSpace.y + 1) * h / 2;
	return PixelSpace;
}

glm::vec3 WindowManager::PixelCoordToScreenSpace(const glm::vec2& PixelSpace) const
{
	glm::vec2 ret = PixelCoordToScreenSpace2D(glm::vec2(PixelSpace.x, PixelSpace.y));
	return glm::vec3(ret.x, ret.y, 0);
}

glm::vec3 WindowManager::ScreenSpaceToPixelCoord(const glm::vec2& ScreenSpace) const
{
	glm::vec2 ret = ScreenSpaceToPixelCoord2D(glm::vec2(ScreenSpace.x, ScreenSpace.y));
	return glm::vec3(ret.x, ret.y, 0);
}

const WindowManager& WindowManager::Get()
{
	Check(Singleton);
	return *Singleton;
}