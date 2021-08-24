#pragma once
#include "../ECS/GameComponent.h"
#include "../Input/InputManager.h"

class RigidbodyComponent : public GameComponent
{
public:
	virtual void OnSpawn() override;
	virtual void OnPostPhysics(float DeltaTime) override;
	virtual void OnPostRender(float DeltaTime) override;

	void Jump(ActionInfo& Info);
	void Right(ActionInfo& Info);
	void Left(ActionInfo& Info);

	void SetDynamic(bool bDynamic);

	bool bDrawDebugPolys = false;

private:
	b2Body* Body;
	b2PolygonShape Box;
	b2Fixture* MainFixture;
	bool bIsDynamic = false;
	bool bSpawned = false;

};