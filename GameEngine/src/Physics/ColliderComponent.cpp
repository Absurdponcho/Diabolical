#include "ColliderComponent.h"
#include "../EngineMain.h"
#include "RigidbodyComponent.h"
#include "PhysicsWorld.h"
std::unordered_map<b2Fixture*, ColliderComponent*> ColliderComponent::FixtureColliderMap;


void ColliderComponent::OnDestroy()
{
	if (Fixture)
	{
		FixtureColliderMap.erase(Fixture);
		RigidbodyComponent* Rigidbody = GetParentEntity()->GetComponent<RigidbodyComponent>();
		if (!Rigidbody) return;
		if (!Rigidbody->GetBody()) return;
		Rigidbody->GetBody()->DestroyFixture(Fixture);
		Fixture = nullptr;
	}
}
void ColliderComponent::OnSpawn()
{
	RigidbodyComponent* Rigidbody = GetParentEntity()->GetComponent<RigidbodyComponent>();
	Check(Rigidbody);

	b2PolygonShape Box;
	Box.SetAsBox(DesiredSize.x, DesiredSize.y);

	b2FixtureDef FixtureDef;
	FixtureDef.shape = &Box;
	FixtureDef.density = DesiredDensity;
	FixtureDef.friction = 0.3f;

	Fixture = Rigidbody->GetBody()->CreateFixture(&FixtureDef);
	Check(Fixture);
	FixtureColliderMap[Fixture] = this;
}

void ColliderComponent::SetSize(b2Vec2 Size)
{
	DesiredSize = Size;
}
void ColliderComponent::SetDensity(float Density)
{
	DesiredDensity = Density;
}