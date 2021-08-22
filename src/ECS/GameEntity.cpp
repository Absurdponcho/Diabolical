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

glm::mat4x4 GameEntity::GetModelMatrix()
{
	glm::mat4x4 TranslateMatrix = glm::translate(glm::mat4x4(1.0f), GetTransform().Position);
	glm::mat4x4 RotateMatrix = glm::mat4_cast(GetTransform().Rotation);
	glm::mat4x4 ScaleMatrix = glm::scale(glm::mat4x4(1.0f), GetTransform().Scale);
	return TranslateMatrix * RotateMatrix * ScaleMatrix;
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

void EntityTransform::SetEulerRotation(glm::vec3 Euler)
{
	Rotation = glm::quat(Euler);
}

glm::vec3 EntityTransform::GetEulerRotation()
{
	return glm::eulerAngles(Rotation);
}
