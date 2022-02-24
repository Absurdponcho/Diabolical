#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <box2d/b2_math.h>
#include <glm/glm.hpp>

class DWindowManager
{
public:
	static void Initialize(const char* WindowTitle, int x, int y, int w, int h, Uint32 WindowFlags);
	static DWindowManager& Get();

	static SDL_Window* GetSDLWindow();
	static SDL_GLContext GetGLContext();
	bool IsValid();

	glm::vec2 PixelCoordToScreenSpace2D(const glm::vec2& PixelSpace);
	glm::vec2 ScreenSpaceToPixelCoord2D(const glm::vec2& ScreenSpace);
	glm::vec3 PixelCoordToScreenSpace(const glm::vec2& PixelSpace);
	glm::vec3 ScreenSpaceToPixelCoord(const glm::vec2& ScreenSpace);

	glm::ivec2 GetScreenSize();
	void SetScreenSize(glm::ivec2 Size);
	void RefreshViewport();

private:
	DWindowManager(const char* WindowTitle, int x, int y, int w, int h, Uint32 WindowFlags);
	~DWindowManager();

	Uint32 WindowFlags = 0;
	bool bWindowValid = false;

	static DWindowManager* Singleton;

	static SDL_Window* GameWindow;
	static SDL_GLContext GameGLContext;

	static void FixWindowsHighDPIScaling();
};
