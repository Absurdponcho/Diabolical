#include "GameComponent.h"
#include "../GunGame.h"
#include "../Logging/Logging.h"

void GameComponent::OnSpawn()
{
	GameBaseObject::OnSpawn();
	Check(ParentEntity);
}

void GameComponent::OnDestroy()
{
	GameBaseObject::OnDestroy();
	if (ParentEntity)
	{
		ParentEntity->DetachComponent(this);
	}
}

void GameComponent::SetParentEntity(GameEntity* Entity)
{
	Check(!GameBaseObject::bCreationLock);
	ParentEntity = Entity;
	ParentEntity->AttachComponent(this);
}