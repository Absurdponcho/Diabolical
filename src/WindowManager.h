#pragma once
#include <Windows.h>
#include <string>
#include <SDL2/SDL.h>
#include <box2d/b2_math.h>
#include <glm/glm.hpp>

static SDL_Window* GameWindow;
static SDL_Surface* GameSurface;
static SDL_Renderer* GameRenderer;
class WindowManager
{
public:
	static void Initialize(const char* WindowTitle, int x, int y, int w, int h, Uint32 WindowFlags);
	static const WindowManager& Get();

	static SDL_Window* GetSDLWindow();
	static SDL_Surface* GetSDLSurface();
	static SDL_Renderer* GetSDLRenderer();
	bool IsValid();

	glm::vec2 PixelCoordToScreenSpace2D(const glm::vec2& PixelSpace) const;
	glm::vec2 ScreenSpaceToPixelCoord2D(const glm::vec2& ScreenSpace) const;
	glm::vec3 PixelCoordToScreenSpace(const glm::vec2& PixelSpace) const;
	glm::vec3 ScreenSpaceToPixelCoord(const glm::vec2& ScreenSpace) const;


private:
	WindowManager(const char* WindowTitle, int x, int y, int w, int h, Uint32 WindowFlags);
	~WindowManager();

	int x = 0, y = 0, w = 0, h = 0;
	Uint32 WindowFlags = 0;
	bool bWindowValid = false;

	static WindowManager* Singleton;
};
