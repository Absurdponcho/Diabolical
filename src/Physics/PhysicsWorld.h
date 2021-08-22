#pragma once
#include <box2d/b2_api.h>
#include <box2d/b2_math.h>
#include <box2d/b2_world.h>

class PhysicsWorld
{
public:
	static PhysicsWorld& Get();
	static void Initialize(b2Vec2 Gravity);

	PhysicsWorld() {}

	void Step();

	b2World& GetWorld();

private:
	static PhysicsWorld* Singleton;

	b2World* World;
};