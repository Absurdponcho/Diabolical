#include "SquareRendererComponent.h"
#include "../WindowManager.h"
#include "../ECS/GameEntity.h"
#include "../Logging/Logging.h"
#include "../Utility/Utility.h"
#include "../GameManager.h"

void SquareRendererComponent::Render()
{
	GameRendererComponent::Render();

	SDL_Renderer* renderer = WindowManager::GetSDLRenderer();
	
	GameEntity& Parent = GetParentEntity();

	b2Vec2 From = b2Vec2(Parent.GetTransform().Position.x - Parent.GetTransform().Scale.x / 2, Parent.GetTransform().Position.y - Parent.GetTransform().Scale.y / 2);
	b2Vec2 To = b2Vec2(Parent.GetTransform().Position.x + Parent.GetTransform().Scale.x / 2, Parent.GetTransform().Position.y + Parent.GetTransform().Scale.y / 2);
	
	From = WindowManager::Get().ScreenSpaceToPixelCoord(From);
	To = WindowManager::Get().ScreenSpaceToPixelCoord(To);

	SDL_Rect Rect;
	Rect.x = (int)From.x;
	Rect.y = (int)From.y;
	Rect.w = (int)(To.x - From.x);
	Rect.h = (int)(To.y - From.y);

	Check(!SDL_RenderFillRect(renderer, &Rect));

	Parent.GetTransform().Position = b2Vec2((float)sin(GameManager::GetTime() * 5) / 2, (float)cos(GameManager::GetTime() * 5) / 2);
}