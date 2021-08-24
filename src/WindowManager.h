#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <box2d/b2_math.h>
#include <glm/glm.hpp>

class WindowManager
{
public:
	static void Initialize(const char* WindowTitle, int x, int y, int w, int h, Uint32 WindowFlags, bool bUseOpenGL);
	static WindowManager& Get();

	static SDL_Window* GetSDLWindow();
	static SDL_Surface* GetSDLSurface();
	static SDL_Renderer* GetSDLRenderer();
	static SDL_GLContext GetGLContext();
	bool IsValid();

	glm::vec2 PixelCoordToScreenSpace2D(const glm::vec2& PixelSpace);
	glm::vec2 ScreenSpaceToPixelCoord2D(const glm::vec2& ScreenSpace);
	glm::vec3 PixelCoordToScreenSpace(const glm::vec2& PixelSpace);
	glm::vec3 ScreenSpaceToPixelCoord(const glm::vec2& ScreenSpace);

	glm::ivec2 GetScreenSize();

private:
	WindowManager(const char* WindowTitle, int x, int y, int w, int h, Uint32 WindowFlags, bool bUseOpenGL);
	~WindowManager();

	Uint32 WindowFlags = 0;
	bool bWindowValid = false;
	bool bUsingOpenGL = false;

	static WindowManager* Singleton;

	static SDL_Window* GameWindow;
	static SDL_Surface* GameSurface;
	static SDL_Renderer* GameRenderer;
	static SDL_GLContext GameGLContext;
};
