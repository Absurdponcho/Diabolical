#pragma once
#include "../ECS/GameComponent.h"
#include "../Input/InputManager.h"

class RigidbodyComponent : public GameComponent
{
public:
	virtual void OnSpawn() override;
	virtual void OnPostPhysics(float DeltaTime) override;
	virtual void OnPostRender(float DeltaTime) override;

	void AddForceAtCenter(const b2Vec2& Force);

	void Jump(ActionInfo Info);
	void Right(ActionInfo Info);
	void Left(ActionInfo Info);

	void SetDynamic(bool bDynamic);
	void SetRotates(bool bRotates);
	void SetLinearDamping(float Damping);
	void SetHorizonalDamping(float Damping);
	void SetVelocity(b2Vec2 Velocity);
	b2Vec2 GetVelocity();


	bool bDrawDebugPolys = false;

private:
	b2Body* Body;
	b2PolygonShape Box;
	b2Fixture* MainFixture;
	float DesiredLinearDamping = .3f;
	float HorizontalDamping = 0;
	bool bIsDynamic = false;
	bool bIsRotating = true;
	bool bSpawned = false;

};