#pragma once
#include "../ECS/GameComponent.h"

class CameraComponent;

class GameRendererComponent : public GameComponent
{
public:
	virtual void OnSpawn() override;
	virtual void OnDestroy() override;

	// if Cull() returns true, renderer is culled and so Render() is not called
	virtual bool Cull(CameraComponent& Camera) { return false; };
	virtual void Render(CameraComponent& Camera) {};

	// Dont call these or u will die ;)
	static void CullAllRenderers(CameraComponent& Camera, Out std::stack<GameRendererComponent*>& VisibleRenderers);
	static void RenderAllRenderers(CameraComponent& Camera, std::stack<GameRendererComponent*>& VisibleRenderers);
private:
	bool bWasCulled = false;

	static std::vector<GameRendererComponent*> AllGameRendererComponents;

};