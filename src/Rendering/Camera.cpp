#include "Camera.h"
#include <box2d/b2_math.h>
#include "../Utility/GameMath.h"
#include "GameRendererComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../GameManager.h"

CameraComponent* CameraComponent::ActiveCamera;

CameraComponent::~CameraComponent()
{
	if (CameraComponent::GetActiveCamera() == this)
	{
		ActiveCamera = nullptr;
	}

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
	GameEntity& Parent = GetParentEntity();
	EntityTransform& Transform = Parent.GetTransform();

	return glm::ortho(-OrthographicSize/2, OrthographicSize / 2, -OrthographicSize / 2, OrthographicSize / 2);
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

	GetParentEntity().GetTransform().SetEulerRotation(glm::vec3(0, 0, GameManager::GetTime()));
}
