#include "Camera.h"
#include <box2d/b2_math.h>
#include "../Utility/GameMath.h"
#include "GameRendererComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../GameManager.h"
#include "../WindowManager.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include "../Logging/Logging.h"


CameraComponent* CameraComponent::ActiveCamera;

void CameraComponent::OnDestroy()
{
	GameComponent::OnDestroy();
	if (CameraComponent::GetActiveCamera() == this)
	{
		ActiveCamera = nullptr;
	}

}

void CameraComponent::OnSpawn()
{
}

void CameraComponent::MoveUp(ActionInfo& Info)
{

}

void CameraComponent::MoveRight(ActionInfo& Info)
{

}

void CameraComponent::MoveLeft(ActionInfo& Info)
{

}

void CameraComponent::MoveDown(ActionInfo& Info)
{

}

void CameraComponent::SetActiveCamera()
{
	ActiveCamera = this;
}

CameraComponent* CameraComponent::GetActiveCamera()
{
	return ActiveCamera;
}

float CameraComponent::GetOrthographicSize()
{
	return OrthographicSize;
}

void CameraComponent::SetOrthographicSize(float Size)
{
	OrthographicSize = Size;
}

glm::mat4x4 CameraComponent::GetProjectionMatrix()
{
	float AspectRatio = GetAspectRatio();

	return glm::ortho(-OrthographicSize/2 * AspectRatio, OrthographicSize / 2 * AspectRatio, -OrthographicSize / 2, OrthographicSize / 2);
}

float CameraComponent::GetAspectRatio()
{
	glm::vec2 ScreenSize = WindowManager::Get().GetScreenSize();
	return ScreenSize.x / ScreenSize.y;
}

glm::mat4x4 CameraComponent::GetViewMatrix()
{
	glm::vec3 ForwardVector = glm::vec3(0, 0, -1) * GetParentEntity().GetTransform().Rotation;
	glm::vec3 UpVector = glm::vec3(0, 1, 0) * GetParentEntity().GetTransform().Rotation;
	return glm::lookAt(GetParentEntity().GetTransform().Position, GetParentEntity().GetTransform().Position + ForwardVector, UpVector);
}

void CameraComponent::Draw()
{
	std::stack<GameRendererComponent*> RendererStack;

	GameRendererComponent::CullAllRenderers(*this, RendererStack);
	GameRendererComponent::RenderAllRenderers(*this, RendererStack);


}
