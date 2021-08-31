#pragma once
#include "../ECS/GameComponent.h"
#include <box2d/b2_api.h>
#include "../Delegate/GameDelegate.h"

class ColliderComponent;
typedef GameMulticastDelegate<ColliderComponent*, ColliderComponent*, b2Contact*> CollisionDelegate;

class ColliderComponent : public GameComponent
{
public:
	~ColliderComponent();
	void SetSize(b2Vec2 Size);
	void SetDensity(float Density);

	void OnStartCollision(ColliderComponent* Other, b2Contact* Contact)
	{
		StartCollisionDelegate.Execute(this, Other, Contact);
	}

	void OnEndCollision(ColliderComponent* Other, b2Contact* Contact)
	{
		EndCollisionDelegate.Execute(this, Other, Contact);
	}

	CollisionDelegate StartCollisionDelegate;
	CollisionDelegate EndCollisionDelegate;


	virtual void OnSpawn() override;
	//virtual void OnDestroy() override;
	static std::unordered_map<b2Fixture*, ColliderComponent*> FixtureColliderMap;

private:
	b2Fixture* Fixture;
	b2Vec2 DesiredSize = b2Vec2(.5f, .5f);
	float DesiredDensity = 1.f;
};