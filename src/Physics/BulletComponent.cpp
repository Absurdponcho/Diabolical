#include "BulletComponent.h"
#include "..\Rendering\Camera.h"
#include "../Utility/Utility.h"

void BulletComponent::OnSpawn()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	RigidbodyPointer = GameWeakObjectPointer<RigidbodyComponent>(GetParentEntity().GetComponent<RigidbodyComponent>());
	ColliderPointer = GameWeakObjectPointer<ColliderComponent>(GetParentEntity().GetComponent<ColliderComponent>());
	glm::vec2 ScreenSpace = { x, y };
	glm::vec3 WorldSpace = Utility::ScreenToWorld(ScreenSpace, CameraComponent::GetActiveCamera());
	glm::vec3 BulletPos = GetParentEntity().GetTransform().GetPosition();
    DirectionVector = WorldSpace - BulletPos;
	ForceVector = DirectionVector * ForceCoefficient;
	RigidbodyPointer.Get()->AddForceAtCenter(Utility::ConvertTob2Vec2(ForceVector));
}

void BulletComponent::SetForceCoefficient(float f)
{
	ForceCoefficient = f;
}
