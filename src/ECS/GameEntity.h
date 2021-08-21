#pragma once
#include "GameBaseObject.h"
#include <box2D/box2D.h>
#include "../GunGame.h"

class GameComponent;

struct EntityTransform
{
	b2Vec2 Position = b2Vec2(0, 0);
	float Rotation = 0;
	b2Vec2 Scale = b2Vec2(1, 1);
};

class GameEntity : public GameBaseObject
{
public:
	virtual void OnTick(float DeltaTime) override;
	virtual void Destroy() final override;

	EntityTransform& GetTransform();


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
