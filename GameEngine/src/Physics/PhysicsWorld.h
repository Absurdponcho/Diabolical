#pragma once
#include <box2d/b2_api.h>
#include <box2d/b2_math.h>
#include <box2d/b2_world.h>

class PhysicsWorld : public b2ContactListener
{
public:
	static PhysicsWorld& Get();
	static void Initialize(b2Vec2 Gravity);

	PhysicsWorld() {}

	void Step();

	b2World& GetWorld();

	virtual void BeginContact(b2Contact* contact) override;
	virtual void EndContact(b2Contact* contact) override;

private:
	static PhysicsWorld* Singleton;

	b2World* World;
};