#pragma once
#include "..\ECS\GameComponent.h"
#include "RigidbodyComponent.h"
#include "ColliderComponent.h"
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
};

