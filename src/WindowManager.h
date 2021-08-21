#pragma once
#include <Windows.h>
#include <string>
#include <SDL2/SDL.h>
#include <box2d/b2_math.h>

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

	b2Vec2 PixelCoordToScreenSpace(const b2Vec2& PixelSpace) const;
	b2Vec2 ScreenSpaceToPixelCoord(const b2Vec2& ScreenSpace) const;


private:
	WindowManager(const char* WindowTitle, int x, int y, int w, int h, Uint32 WindowFlags);
	~WindowManager();

	int x = 0, y = 0, w = 0, h = 0;
	Uint32 WindowFlags = 0;
	bool bWindowValid = false;

	static WindowManager* Singleton;
};
