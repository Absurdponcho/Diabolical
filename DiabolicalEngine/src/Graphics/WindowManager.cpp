#include "WindowManager.h"
#include <gl/glew.h>
#include "../Logging/Logging.h"
#include "../DiabolicalEngine.h"

#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#endif

DWindowManager* DWindowManager::Singleton;
SDL_Window* DWindowManager::GameWindow;
SDL_GLContext DWindowManager::GameGLContext;

DWindowManager::DWindowManager(const char* title, int x, int y, int w, int h, Uint32 flags) :
	WindowFlags(flags), bWindowValid(true)
{
	
	FixWindowsHighDPIScaling();

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		LOG_ERR("SDL_Init() failed!");
		bWindowValid = false;
	}
	LOG("SDL_Init() success");

	GameWindow = SDL_CreateWindow(title, x, y, w, h, flags | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
	GameGLContext = SDL_GL_CreateContext(GameWindow);
	if (!GameGLContext) {
		LOG_ERR("SDL_Init() failed!");
		bWindowValid = false;
	}
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}	

void DWindowManager::Initialize(const char* WindowTitle, int x, int y, int w, int h, Uint32 WindowFlags)
{
	static bool bInitialized = false;

	Check(!bInitialized);
	bInitialized = true;
	Singleton = new DWindowManager(WindowTitle, x, y, w, h, WindowFlags);
}

void DWindowManager::FixWindowsHighDPIScaling()
{

#ifdef PLATFORM_WINDOWS
	typedef enum PROCESS_DPI_AWARENESS {
		PROCESS_DPI_UNAWARE = 0,
		PROCESS_SYSTEM_DPI_AWARE = 1,
		PROCESS_PER_MONITOR_DPI_AWARE = 2
	} PROCESS_DPI_AWARENESS;

	void* userDLL;
	BOOL(WINAPI * SetProcessDPIAware)(void) = nullptr; // Vista and later
	void* shcoreDLL;
	HRESULT(WINAPI * SetProcessDpiAwareness)(PROCESS_DPI_AWARENESS dpiAwareness) = nullptr; // Windows 8.1 and later

	userDLL = SDL_LoadObject("USER32.DLL");
	if (userDLL) {
		SetProcessDPIAware = (BOOL(WINAPI*)(void)) SDL_LoadFunction(userDLL, "SetProcessDPIAware");
	}

	shcoreDLL = SDL_LoadObject("SHCORE.DLL");
	if (shcoreDLL) {
		SetProcessDpiAwareness = (HRESULT(WINAPI*)(PROCESS_DPI_AWARENESS)) SDL_LoadFunction(shcoreDLL, "SetProcessDpiAwareness");
	}

	if (SetProcessDpiAwareness) {
		/* Try Windows 8.1+ version */
		HRESULT result = SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
		SDL_Log("called SetProcessDpiAwareness: %d", (result == S_OK) ? 1 : 0);
	}
	else if (SetProcessDPIAware) {
		/* Try Vista - Windows 8 version.
		This has a constant scale factor for all monitors.
		*/
		BOOL success = SetProcessDPIAware();
		SDL_Log("called SetProcessDPIAware: %d", (int)success);
	}
#endif // PLATFORM_WINDOWS
}

DWindowManager::~DWindowManager()
{
	SDL_GL_DeleteContext(GameGLContext);
	SDL_DestroyWindow(GameWindow);
	SDL_Quit();
}

SDL_Window* DWindowManager::GetSDLWindow()
{
	Check(GameWindow);
	return GameWindow;
}

SDL_GLContext DWindowManager::GetGLContext()
{
	Check(GameGLContext);
	return GameGLContext;
}

bool DWindowManager::IsValid()
{
	return bWindowValid;
}

glm::vec2 DWindowManager::PixelCoordToScreenSpace2D(const glm::vec2& PixelSpace)
{
	glm::ivec2 ScreenSize = GetScreenSize();
	glm::vec2 ScreenSpace;
	ScreenSpace.x = ((PixelSpace.x / ScreenSize.x) - .5f) * 2;
	ScreenSpace.y = (((ScreenSize.y - PixelSpace.y) / ScreenSize.y) - .5f) * 2;
	return ScreenSpace;
}

glm::vec2 DWindowManager::ScreenSpaceToPixelCoord2D(const glm::vec2& ScreenSpace)
{
	glm::ivec2 ScreenSize = GetScreenSize();
	glm::vec2 PixelSpace;
	PixelSpace.x = (ScreenSpace.x + 1) * ScreenSize.x / 2;
	PixelSpace.y = ScreenSize.y - ((ScreenSpace.y + 1) * ScreenSize.y / 2);
	return PixelSpace;
}

glm::vec3 DWindowManager::PixelCoordToScreenSpace(const glm::vec2& PixelSpace)
{
	glm::vec2 ret = PixelCoordToScreenSpace2D(glm::vec2(PixelSpace.x, PixelSpace.y));
	return glm::vec3(ret.x, ret.y, 0);
}

glm::vec3 DWindowManager::ScreenSpaceToPixelCoord(const glm::vec2& ScreenSpace)
{
	glm::vec2 ret = ScreenSpaceToPixelCoord2D(glm::vec2(ScreenSpace.x, ScreenSpace.y));
	return glm::vec3(ret.x, ret.y, 0);
}

DWindowManager& DWindowManager::Get()
{
	Check(Singleton);
	return *Singleton;
}

glm::ivec2 DWindowManager::GetScreenSize()
{
	int Width;
	int Height;
	SDL_GetWindowSize(GetSDLWindow(), &Width, &Height);
	return glm::ivec2(Width, Height);
}