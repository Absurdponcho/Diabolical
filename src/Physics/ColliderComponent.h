#pragma once
#include "../ECS/GameComponent.h"
#include <box2d/b2_api.h>

class ColliderComponent : public GameComponent
{
public:
	void SetSize(b2Vec2 Size);
	void SetDensity(float Density);
	virtual void OnSpawn() override;

private:
	b2Vec2 DesiredSize = b2Vec2(.5f, .5f);
	float DesiredDensity = 1.f;
};