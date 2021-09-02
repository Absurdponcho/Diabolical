#pragma once
#include <ECS/GameComponent.h>
#include <Physics/RigidbodyComponent.h>
#include <Physics/ColliderComponent.h>
class BulletComponent : public GameComponent
{
public:
	virtual void OnSpawn() override;
	virtual void SetForceCoefficient(float f);
private:
	GameWeakObjectPointer<RigidbodyComponent> RigidbodyPointer;
	GameWeakObjectPointer<ColliderComponent> ColliderPointer;
	float ForceCoefficient;
	glm::vec2 ForceVector;
	glm::vec2 DirectionVector;

	void OnHitSomething(ColliderComponent* Self, ColliderComponent* Other, b2Contact* Contact);
};

