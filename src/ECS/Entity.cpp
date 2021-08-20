#include "Entity.h"
#include "../Logging/Logging.h"

void Entity::RemoveComponent(Component* Comp)
{
	for (int Index = 0; Index < Components.size(); Index++)
	{
		if (Comp == Components[Index])
		{
			Components.erase(Components.begin() + Index);
			Logging::Log("Entity::RemoveComponent()", "Erased Component");
			return;
		}
	}

	Logging::Log("Entity::RemoveComponent()", "Tried to erase Component but could not find it");
}