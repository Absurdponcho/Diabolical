#include "GameEntity.h"
#include "../Logging/Logging.h"
#include "GameComponent.h"
#include <glm/gtx/euler_angles.hpp>
#include "../GameManager.h"

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

glm::mat4x4 EntityTransform::GetModelMatrix()
{
	if (bModelMatrixDirty)
	{
		glm::mat4x4 TranslateMatrix = glm::translate(glm::mat4x4(1.0f), Position);
		glm::mat4x4 RotateMatrix = glm::toMat4(Rotation);
		glm::mat4x4 ScaleMatrix = glm::scale(glm::mat4x4(1.0f), Scale);
		CachedModelMatrix = TranslateMatrix * RotateMatrix * ScaleMatrix;
		bModelMatrixDirty = false;
	}
	return CachedModelMatrix;
}

glm::mat4x4 EntityTransform::GetXMirroredModelMatrix()
{
	if (bXMirroredModelMatrixDirty)
	{
		glm::mat4x4 TranslateMatrix = glm::translate(glm::mat4x4(1.0f), Position);
		glm::mat4x4 RotateMatrix = glm::toMat4(Rotation);
		glm::mat4x4 ScaleMatrix = glm::scale(glm::mat4x4(1.0f), Scale * glm::vec3(-1, 1, 1));
		CachedXMirroredModelMatrix = TranslateMatrix * RotateMatrix * ScaleMatrix;
	}
	return CachedXMirroredModelMatrix;
}

glm::mat4x4 EntityTransform::GetTransRotationMatrix()
{
	if (bTransRotationMatrixDirty)
	{
		glm::mat4x4 TranslateMatrix = glm::translate(glm::mat4x4(1.0f), Position);
		glm::mat4x4 RotateMatrix = glm::toMat4(Rotation);
		CachedTransRotationMatrix = TranslateMatrix * RotateMatrix;
	}
	return CachedTransRotationMatrix;
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
	Euler *= 0.0174533f; // deg to rad
	Rotation = glm::quat(Euler);

	bModelMatrixDirty = true;
	bXMirroredModelMatrixDirty = true;
	bTransRotationMatrixDirty = true;
}

glm::vec3 EntityTransform::GetEulerRotation()
{
	return glm::eulerAngles(Rotation);
}

void EntityTransform::SetPosition(glm::vec3 NewPosition)
{
	Position = NewPosition;

	bModelMatrixDirty = true;
	bXMirroredModelMatrixDirty = true;
	bTransRotationMatrixDirty = true;
}
glm::vec3 EntityTransform::GetPosition()
{
	return Position;
}
void EntityTransform::SetScale(glm::vec3 NewScale)
{
	Scale = NewScale;

	bModelMatrixDirty = true;
	bXMirroredModelMatrixDirty = true;
}
glm::vec3 EntityTransform::GetScale()
{
	return Scale;
}

glm::quat EntityTransform::GetRotation()
{
	return Rotation;
}
