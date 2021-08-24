#include "WindowManager.h"
#include "Logging/Logging.h"
#include "GunGame.h"

WindowManager* WindowManager::Singleton;
SDL_Window* WindowManager::GameWindow;
SDL_Surface* WindowManager::GameSurface;
SDL_Renderer* WindowManager::GameRenderer;
SDL_GLContext WindowManager::GameGLContext;

WindowManager::WindowManager(const char* title, int x, int y, int w, int h, Uint32 flags, bool bUseOpenGL) :
	WindowFlags(flags), bWindowValid(true)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		Logging::LogError("WindowManager::WindowManager()", "SDL_Init() failed!");
		bWindowValid = false;
	}
	Logging::Log("WindowManager::WindowManager()", "SDL_Init() success");

	if (bUseOpenGL) {
		Logging::Log("WindowManager::WindowManager()", "Using OpenGL");
		flags |= SDL_WINDOW_OPENGL;
		bUsingOpenGL = true;
	}

	GameWindow = SDL_CreateWindow(title, x, y, w, h, flags | SDL_WINDOW_RESIZABLE);

	if (bUseOpenGL) {
		GameGLContext = SDL_GL_CreateContext(GameWindow);
	}
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
	else
	{
		// set the screen to black asap cos white screen on boot is icky
		SDL_SetRenderDrawColor(GameRenderer, 0, 0, 0, 255);
		SDL_RenderClear(GameRenderer);
		SDL_RenderPresent(GameRenderer);
	}
}

void WindowManager::Initialize(const char* WindowTitle, int x, int y, int w, int h, Uint32 WindowFlags, bool bUseOpenGL)
{
	static bool bInitialized = false;

	Check(!bInitialized);
	bInitialized = true;
	Singleton = new WindowManager(WindowTitle, x, y, w, h, WindowFlags, bUseOpenGL);
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

SDL_GLContext WindowManager::GetGLContext()
{
	Check(GameGLContext);
	return GameGLContext;
}

bool WindowManager::IsValid()
{
	return bWindowValid;
}

glm::vec2 WindowManager::PixelCoordToScreenSpace2D(const glm::vec2& PixelSpace)
{
	glm::ivec2 ScreenSize = GetScreenSize();
	glm::vec2 ScreenSpace;
	ScreenSpace.x = ((PixelSpace.x / ScreenSize.x) - .5f) * 2;
	ScreenSpace.y = (((ScreenSize.y - PixelSpace.y) / ScreenSize.y) - .5f) * 2;
	return ScreenSpace;
}

glm::vec2 WindowManager::ScreenSpaceToPixelCoord2D(const glm::vec2& ScreenSpace)
{
	glm::ivec2 ScreenSize = GetScreenSize();
	glm::vec2 PixelSpace;
	PixelSpace.x = (ScreenSpace.x + 1) * ScreenSize.x / 2;
	PixelSpace.y = ScreenSize.y - ((ScreenSpace.y + 1) * ScreenSize.y / 2);
	return PixelSpace;
}

glm::vec3 WindowManager::PixelCoordToScreenSpace(const glm::vec2& PixelSpace)
{
	glm::vec2 ret = PixelCoordToScreenSpace2D(glm::vec2(PixelSpace.x, PixelSpace.y));
	return glm::vec3(ret.x, ret.y, 0);
}

glm::vec3 WindowManager::ScreenSpaceToPixelCoord(const glm::vec2& ScreenSpace)
{
	glm::vec2 ret = ScreenSpaceToPixelCoord2D(glm::vec2(ScreenSpace.x, ScreenSpace.y));
	return glm::vec3(ret.x, ret.y, 0);
}

WindowManager& WindowManager::Get()
{
	Check(Singleton);
	return *Singleton;
}

glm::ivec2 WindowManager::GetScreenSize()
{
	int Width;
	int Height;
	SDL_GetWindowSize(GetSDLWindow(), &Width, &Height);
	return glm::ivec2(Width, Height);
}