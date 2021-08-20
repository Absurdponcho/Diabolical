#pragma once
#include "../ECS/GameComponent.h"

class GameRendererComponent : public GameComponent
{
public:
	virtual void OnSpawn() override;
	virtual void OnDestroy() override;

	virtual void Render() {};

	static void RenderAllRenderers();
private:
	static std::vector<GameRendererComponent*> AllGameRendererComponents;
};