#pragma once
#include "GameBaseObject.h"
#include <box2D/box2D.h>
#include "../GunGame.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

class GameComponent;

struct EntityTransform
{
	glm::vec3 Position = glm::vec3(0, 0, 0);
	glm::quat Rotation = glm::quat(1.0, 0.0, 0.0, 0.0);;
	glm::vec3 Scale = glm::vec3(1, 1, 1);

	void SetEulerRotation(glm::vec3 Euler);
	glm::vec3 GetEulerRotation();
};

class GameEntity : public GameBaseObject
{
public:
	virtual void OnTick(float DeltaTime) override;
	virtual void Destroy() final override;

	EntityTransform& GetTransform();

	glm::mat4x4 GetModelMatrix();
	glm::mat4x4 GetTransRotationMatrix();

	// dont call these manually 
	void AttachComponent(GameComponent* Component);
	void DetachComponent(GameComponent* Component);

private:
	EntityTransform Transform = EntityTransform();

	std::vector<GameComponent*> AttachedComponents;
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
