#include "GameEntity.h"
#include "../Logging/Logging.h"
#include "GameComponent.h"

void GameEntity::RemoveComponent(GameComponent* Comp)
{
	for (int Index = 0; Index < Components.size(); Index++)
	{
		if (Comp == Components[Index])
		{
			Components.erase(Components.begin() + Index);
			Logging::Log("GameEntity::RemoveComponent()", "Erased GameComponent");
			return;
		}
	}

	Logging::Log("GameEntity::RemoveComponent()", "Tried to erase GameComponent but could not find it");
}