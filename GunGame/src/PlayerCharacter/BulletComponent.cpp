#include "BulletComponent.h"
#include <Rendering\Camera.h>
#include <Utility/Utility.h>
#include <Physics/ColliderComponent.h>
#include "PlayerCharacterEntity.h"

void BulletComponent::OnSpawn()
{
	if (ColliderComponent* Collider = GetParentEntity()->GetComponent<ColliderComponent>())
	{
		Collider->StartCollisionDelegate.Bind(this, &BulletComponent::OnHitSomething);
	}
	/*int x, y;
	SDL_GetMouseState(&x, &y);
	RigidbodyPointer = GameWeakObjectPointer<RigidbodyComponent>(GetParentEntity()->GetComponent<RigidbodyComponent>());
	ColliderPointer = GameWeakObjectPointer<ColliderComponent>(GetParentEntity()->GetComponent<ColliderComponent>());
	glm::vec2 ScreenSpace = { x, y };
	glm::vec3 WorldSpace = Utility::ScreenToWorld(ScreenSpace, CameraComponent::GetActiveCamera());
	glm::vec3 BulletPos = GetParentEntity()->GetTransform().GetPosition();
    DirectionVector = WorldSpace - BulletPos;
	ForceVector = DirectionVector * ForceCoefficient;
	RigidbodyPointer.Get()->AddForceAtCenter(Utility::ConvertTob2Vec2(ForceVector));*/
}

void BulletComponent::OnHitSomething(ColliderComponent* Self, ColliderComponent* Other, b2Contact* Contact)
{
	if (!Other) return;
	if (dynamic_cast<PlayerCharacterEntity*>(Other->GetParentEntity())) return;
	if (Other->GetParentEntity()->GetComponent<BulletComponent>()) return;
	if (IsPendingDestroy()) return;
	
	GetParentEntity()->Destroy();
	
}

void BulletComponent::SetForceCoefficient(float f)
{
	ForceCoefficient = f;
}
