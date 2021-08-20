#include "GameComponent.h"
#include "../GunGame.h"
#include "../Logging/Logging.h"

void GameComponent::OnSpawn()
{
	GameBaseObject::OnSpawn();
	Check(ParentEntity);
}

void GameComponent::SetParentEntity(GameEntity* Entity)
{
	Check(!GameBaseObject::bCreationLock);
	ParentEntity = Entity;
}