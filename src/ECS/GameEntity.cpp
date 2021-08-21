#include "GameEntity.h"
#include "../Logging/Logging.h"
#include "GameComponent.h"

void GameEntity::OnTick(float DeltaTime)
{
	GameBaseObject::OnTick(DeltaTime);
	//Logging::LogVerbose("GameEntity::OnTick()", "Ticked");
}
void GameEntity::Destroy()
{
	for (int Index = (int)AttachedComponents.size() - 1; Index >= 0; Index--)
	{
		if (!AttachedComponents[Index]->IsPendingDestroy())
		{
			AttachedComponents[Index]->Destroy();
		}
	}
	GameBaseObject::Destroy();

}

EntityTransform& GameEntity::GetTransform()
{
	return Transform;
}

void GameEntity::AttachComponent(GameComponent* Component)
{
	Check(Component);
	Check(!GameBaseObject::bCreationLock);

	AttachedComponents.push_back(Component);

	Logging::LogVerbose("GameEntity::AttachComponent()", "Component (" + std::to_string(Component->GetUID()) + ") attached to entity (" + std::to_string(GetUID()) + ")");
}

void GameEntity::DetachComponent(GameComponent* Component)
{
	for (int Index = 0; Index < AttachedComponents.size(); Index++)
	{
		if (Component->GetUID() == AttachedComponents[Index]->GetUID())
		{
			AttachedComponents.erase(AttachedComponents.begin() + Index);
			return;
		}
	}
}