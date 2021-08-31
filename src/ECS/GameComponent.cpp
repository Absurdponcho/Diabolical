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
	if (GetParentEntity())
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

GameEntity* GameComponent::GetParentEntity()
{
	if (IsPendingDestroy())
	{
		return nullptr;
	}
	if (ParentEntity->IsPendingDestroy())
	{
		return nullptr;
	}
	Check(ParentEntity);
	return ParentEntity;
}