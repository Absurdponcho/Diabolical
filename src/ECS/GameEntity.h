#pragma once
#include "GameBaseObject.h"
#include <box2D/box2D.h>
#include "../GunGame.h"

struct EntityTransform
{
	b2Vec2 Position = b2Vec2();
	float Rotation = 0;
	b2Vec2 Scale = b2Vec2();
};

class GameEntity : public GameBaseObject
{
public:
	virtual void OnTick(float DeltaTime) override;

	EntityTransform& GetTransform();

private:
	EntityTransform Transform = EntityTransform();
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
