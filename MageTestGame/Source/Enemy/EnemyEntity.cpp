#include "EnemyEntity.h"
#include <Physics/RigidbodyComponent.h>

void EnemyEntity::Damage(float DamageDealt, GameEntity* Causer)
{
	CharacterEntity::Damage(DamageDealt, Causer);

	if (Causer)
	{
		if (RigidbodyComponent* Rigidbody = GetComponent<RigidbodyComponent>())
		{
			glm::vec2 ForceDirection = glm::normalize(GetTransform().GetPosition() - Causer->GetTransform().GetPosition());
			Rigidbody->AddForceAtCenter(Utility::ConvertTob2Vec2(ForceDirection * 400.f));
		}
	}
}