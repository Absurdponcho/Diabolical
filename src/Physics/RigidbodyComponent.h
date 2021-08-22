#pragma once
#include "../ECS/GameComponent.h"

class RigidbodyComponent : public GameComponent
{
public:
	virtual void OnSpawn() override;
	virtual void OnPostPhysics(float DeltaTime) override;
	virtual void OnPostRender(float DeltaTime) override;

	void SetDynamic(bool bDynamic);

	bool bDrawDebugPolys = false;

private:
	b2Body* Body;
	b2PolygonShape Box;
	b2Fixture* MainFixture;
	bool bIsDynamic = false;
	bool bSpawned = false;

};