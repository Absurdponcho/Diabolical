#include "PhysicsWorld.h"
#include "../EngineMain.h"
#include  <box2d/b2_world.h>
#include "../GameManager.h"
#include "../ECS/GameBaseObject.h"
#include "../Logging/Logging.h"
#include <box2d/b2_api.h>
#include <box2d/b2_contact.h>
#include "ColliderComponent.h"
#include "Box2dQueryCallback.h"
#include "RigidbodyComponent.h"

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
	Singleton->World->SetContactListener(Singleton);
}

void PhysicsWorld::QueryAABB(b2AABB AABB, std::vector<GameEntity*>& OutEntities)
{
	// I thought using a query callback means the query would be async but NO...!!
	// Box2d devs are weird
	Box2dQueryCallback B2QueryCallback = Box2dQueryCallback();
	GetWorld().QueryAABB(&B2QueryCallback, AABB);

	for (b2Body* Body : B2QueryCallback.FoundBodies)
	{
		if (RigidbodyComponent* Rigidbody = RigidbodyComponent::BodyRigidbodyMap[Body])
		{
			Check(Rigidbody->GetParentEntity());
			OutEntities.push_back(Rigidbody->GetParentEntity());
		}
	}
}

b2World& PhysicsWorld::GetWorld()
{
	Check(World);
	return *World;
}

void PhysicsWorld::Step()
{
	static float LastStepTime = GameManager::GetTime();

	float TimeStep = 1.0f / 60.0f;

	while (GameManager::GetTime() > LastStepTime + TimeStep) // Make sure the physics simulation is caught up just incase any lag spikes happen
	{
		int32 VelocityIterations = 6;
		int32 PositionIterations = 2;

		GetWorld().Step(TimeStep, VelocityIterations, PositionIterations);
		LastStepTime += TimeStep;

		GameBaseObject::TickPhysics(TimeStep);

	}
}

void PhysicsWorld::BeginContact(b2Contact* contact)
{
	Check(contact->GetFixtureA());
	Check(contact->GetFixtureB());

	ColliderComponent* Collider = ColliderComponent::FixtureColliderMap[contact->GetFixtureA()];
	ColliderComponent* Collider2 = ColliderComponent::FixtureColliderMap[contact->GetFixtureB()];

	//Check(Collider);
	//Check(Collider2);
	if (Collider && Collider2)
	{
		Collider->OnStartCollision(Collider2, contact);
		Collider2->OnStartCollision(Collider, contact);
	}

}
void PhysicsWorld::EndContact(b2Contact* contact)
{
	Check(contact->GetFixtureA());
	Check(contact->GetFixtureB());

	ColliderComponent* Collider = ColliderComponent::FixtureColliderMap[contact->GetFixtureA()];
	ColliderComponent* Collider2 = ColliderComponent::FixtureColliderMap[contact->GetFixtureB()];

	//Check(Collider);
	//Check(Collider2);
	if (Collider && Collider2)
	{
		Collider->OnEndCollision(Collider2, contact);
		Collider2->OnEndCollision(Collider, contact);
	}
}