#pragma once
#include "../ECS/GameComponent.h"

class GameRendererComponent : public GameComponent
{
public:
	virtual void OnSpawn() override;
	virtual void OnDestroy() override;

	// if Cull() returns true, renderer is culled and so Render() is not called
	virtual bool Cull() { return false; };
	virtual void Render() {};

	// Dont call these or u will die ;)
	static void CullAllRenderers(Out std::stack<GameRendererComponent*>& VisibleRenderers);
	static void RenderAllRenderers(std::stack<GameRendererComponent*>& VisibleRenderers);
private:
	bool bWasCulled = false;

	static std::vector<GameRendererComponent*> AllGameRendererComponents;

};