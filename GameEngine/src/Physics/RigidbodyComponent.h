#pragma once
#include "../ECS/GameComponent.h"
#include "../Input/InputManager.h"

class RigidbodyComponent : public GameComponent
{
public:
	virtual void OnSpawn() override;
	virtual void OnPostPhysics(float DeltaTime) override;
	virtual void OnPostRender(float DeltaTime) override;
	virtual void OnDestroy() override;

	void AddForceAtCenter(const b2Vec2& Force);

	void Jump(ActionInfo Info);
	void Right(ActionInfo Info);
	void Left(ActionInfo Info);

	void SetDynamic(bool bDynamic);
	void SetRotates(bool bRotates);
	void SetLinearDamping(float Damping);
	void SetHorizonalDamping(float Damping);
	void SetVelocity(b2Vec2 Velocity);
	void SetUsesGravity(bool bGravity);
	b2Vec2 GetVelocity();

	void SetOffset(glm::vec2 NewOffset);

	b2Body* GetBody();

	bool bDrawDebugPolys = false;

	static std::unordered_map<b2Fixture*, RigidbodyComponent*> FixtureRigidbodyMap;

private:
	b2Body* Body;
	b2Fixture* MainFixture;
	float DesiredLinearDamping = .3f;
	float HorizontalDamping = 0;
	b2Vec2 DesiredVelocity = b2Vec2(0, 0);

	bool bIsDynamic = false;
	bool bIsRotating = true;
	bool bSpawned = false;
	bool bUsesGravity = true;

	// Currently doesnt handle rotation, but it is meant to :P
	glm::vec2 Offset = glm::vec2(0, 0);
};