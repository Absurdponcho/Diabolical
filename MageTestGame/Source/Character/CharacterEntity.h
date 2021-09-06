#pragma once
#include <ECS/GameEntity.h>

class CharacterEntity : public GameEntity
{
public:
	virtual void Die() {};
	virtual void Damage(float DamageDealt, GameEntity* Causer);

private:
	float Health = 100;
	float MaxHealth = 100;
};