#include "BasicRendererComponent.h"
BasicRendererComponent::BasicRendererComponent() {}

void BasicRendererComponent::Render(CameraComponent& Camera)
{
	SDL_Renderer* renderer = WindowManager::GetSDLRenderer();
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	for (int i = 0; i < 600; ++i)
		SDL_RenderDrawPoint(renderer, i, i);
	
}