#include "CharacterEntity.h"

void CharacterEntity::Damage(float DamageDealt, GameEntity* Causer)
{
	Health -= DamageDealt;
	if (Health <= 0) Die();
}