#include "PhysicsWorld.h"
#include "../GunGame.h"
#include  <box2d/b2_world.h>
#include "../GameManager.h"
PhysicsWorld* PhysicsWorld::Singleton;

PhysicsWorld& PhysicsWorld::Get()
{
	Check(Singleton);
	return *Singleton;
}

void PhysicsWorld::Initialize(b2Vec2 Gravity)
{
	Check(!Singleton);
	Singleton = new PhysicsWorld();
	Singleton->World = new b2World(Gravity);
}

b2World& PhysicsWorld::GetWorld()
{
	Check(World);
	return *World;
}

void PhysicsWorld::Step()
{
	static float LastStepTime = GameManager::GetTime();
	float TimeStep = 1.0f / 30.0f;

	while (GameManager::GetTime() > LastStepTime + TimeStep) // Make sure the physics simulation is caught up just incase any lag spikes happen
	{
		int32 VelocityIterations = 6;
		int32 PositionIterations = 2;

		GetWorld().Step(TimeStep, VelocityIterations, PositionIterations);

		LastStepTime += TimeStep;
	}
}