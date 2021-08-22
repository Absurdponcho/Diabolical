#include "SquareRendererComponent.h"
#include "../WindowManager.h"
#include "../ECS/GameEntity.h"
#include "../Logging/Logging.h"
#include "../Utility/Utility.h"
#include "../GameManager.h"
#include "../Rendering/Camera.h"
#include <glm/glm.hpp>

void SquareRendererComponent::Render(CameraComponent& Camera)
{
	GameRendererComponent::Render(Camera);

	SDL_Renderer* renderer = WindowManager::GetSDLRenderer();
	
	GameEntity& Parent = GetParentEntity();

	glm::vec4 From = glm::vec4(Parent.GetTransform().Position.x - Parent.GetTransform().Scale.x / 2, Parent.GetTransform().Position.y - Parent.GetTransform().Scale.y / 2, 0, 1);
	glm::vec4 To = glm::vec4(Parent.GetTransform().Position.x + Parent.GetTransform().Scale.x / 2, Parent.GetTransform().Position.y + Parent.GetTransform().Scale.y / 2, 0, 1);

	glm::mat4x4 ModelMatrix = Parent.GetModelMatrix();
	glm::mat4 ViewMatrix = Camera.GetViewMatrix();
	glm::mat4x4 ProjectionMatrix = Camera.GetProjectionMatrix();

	glm::mat4x4 MVPMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;

	From = MVPMatrix * From;
	To = MVPMatrix * To;

	From = glm::vec4(WindowManager::Get().ScreenSpaceToPixelCoord(From), 0);
	To = glm::vec4(WindowManager::Get().ScreenSpaceToPixelCoord(To), 0);

	SDL_Rect Rect;
	Rect.x = (int)From.x;
	Rect.y = (int)From.y;
	Rect.w = (int)(To.x - From.x);
	Rect.h = (int)(To.y - From.y);

	Check(!SDL_RenderFillRect(renderer, &Rect));
}