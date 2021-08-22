#pragma once
#include "../ECS/GameComponent.h"

class RigidbodyComponent : public GameComponent
{
public:
	virtual void OnSpawn() override;
	virtual void OnPostPhysics(float DeltaTime) override;

	void SetDynamic(bool bDynamic);

private:
	b2Body* Body;
	b2PolygonShape Box;
	bool bIsDynamic = false;
	bool bSpawned = false;
};