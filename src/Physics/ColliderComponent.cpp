#include "ColliderComponent.h"
#include "../GunGame.h"
#include "RigidbodyComponent.h"

void ColliderComponent::OnSpawn()
{
	RigidbodyComponent* Rigidbody = GetParentEntity().GetComponent<RigidbodyComponent>();
	Check(Rigidbody);

	b2PolygonShape Box;
	Box.SetAsBox(DesiredSize.x, DesiredSize.y);

	b2FixtureDef FixtureDef;
	FixtureDef.shape = &Box;
	FixtureDef.density = DesiredDensity;
	FixtureDef.friction = 0.3f;

	b2Fixture* Fixture = Rigidbody->GetBody()->CreateFixture(&FixtureDef);
}

void ColliderComponent::SetSize(b2Vec2 Size)
{
	DesiredSize = Size;
}
void ColliderComponent::SetDensity(float Density)
{
	DesiredDensity = Density;
}