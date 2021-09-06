#pragma once
#include <ECS/GameEntity.h>
#include "../Character/CharacterEntity.h"

class EnemyEntity : public CharacterEntity
{
public:
	virtual void Damage(float DamageDealt, GameEntity* Causer) override;
	virtual void Die() override { Destroy(); }
};