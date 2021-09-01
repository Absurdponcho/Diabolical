#pragma once
#include "GameBaseObject.h"
#include <box2D/box2D.h>
#include "../GunGame.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "../ECS/GameWeakObjectPointer.h"

class GameComponent;
class GameEntity;

struct EntityTransform
{
public:
	EntityTransform(GameEntity* NewParent) : Parent(NewParent){};

	void SetEulerRotation(glm::vec3 Euler);
	glm::vec3 GetEulerRotation();
	glm::vec3 GetWorldEulerRotation();

	void SetRotation(glm::quat NewRotation);
	glm::quat GetRotation();
	glm::quat GetWorldRotation();

	void SetPosition(glm::vec3 NewPosition);
	glm::vec3 GetPosition();
	glm::vec3 GetWorldPosition();

	void SetScale(glm::vec3 NewScale);
	glm::vec3 GetScale();

	glm::mat4x4 GetModelMatrix();
	glm::mat4x4 GetTransRotationMatrix();

	glm::vec2 ForwardVector2D();

	void AttachTo(GameEntity* Entity);
	void SetDirtyMatrixCache();

private:
	glm::vec3 Position = glm::vec3(0, 0, 0);
	glm::quat Rotation = glm::quat(1.0, 0.0, 0.0, 0.0);
	glm::vec3 Scale = glm::vec3(1, 1, 1);
	GameWeakObjectPointer<GameEntity> AttachedEntity;

	void AddChild(GameWeakObjectPointer<GameEntity>);

	std::vector<GameWeakObjectPointer<GameEntity>> Children;

	GameEntity* Parent;


	bool bModelMatrixDirty = false;
	glm::mat4x4 CachedModelMatrix;
	bool bTransRotationMatrixDirty = false;
	glm::mat4x4 CachedTransRotationMatrix;
};

class GameEntity : public GameBaseObject
{
public:
	virtual void OnTick(float DeltaTime) override;
	virtual void Destroy() final override;

	EntityTransform& GetTransform();

	template <class UserClass>
	UserClass* GetComponent()
	{
		for (GameComponent* Component : AttachedComponents)
		{
			if (UserClass* Comp = dynamic_cast<UserClass*>(Component))
			{
				return Comp;
			}
		}
		return nullptr;
	}



	// dont call these manually 
	void AttachComponent(GameComponent* Component);
	void DetachComponent(GameComponent* Component);

private:


	EntityTransform Transform = EntityTransform(this);

	std::vector<GameComponent*> AttachedComponents;
	std::vector<GameEntity*> AttachedEntites;
};


template <class T>
T* CreateEntity()
{
	GameBaseObject::bCreationLock = false;
	T* Object = new T();

	Check(dynamic_cast<GameEntity*>(Object) != nullptr);

	GameBaseObject::bCreationLock = true;
	return Object;
};
