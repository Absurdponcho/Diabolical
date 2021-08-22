#include "GameRendererComponent.h"
#include "../Logging/Logging.h"
#include "Camera.h"

std::vector<GameRendererComponent*> GameRendererComponent::AllGameRendererComponents;

void GameRendererComponent::OnSpawn()
{
	GameComponent::OnSpawn();
	AllGameRendererComponents.push_back(this);
}

void GameRendererComponent::OnDestroy()
{
	GameComponent::OnDestroy();

	for (int Index = 0; Index < AllGameRendererComponents.size(); Index++)
	{
		if (*this == *AllGameRendererComponents[Index])
		{
			AllGameRendererComponents.erase(AllGameRendererComponents.begin() + Index);
			return;
		}
	}
}
void GameRendererComponent::RenderAllRenderers(CameraComponent& Camera, std::stack<GameRendererComponent*>& VisibleRenderers)
{
	while (!VisibleRenderers.empty())
	{
		GameRendererComponent* RendererComponent = VisibleRenderers.top();
		if (RendererComponent->IsPendingDestroy() || !RendererComponent->IsEnabled())
		{
			continue;
		}
		RendererComponent->Render(Camera);
		VisibleRenderers.pop();
	}
}
void GameRendererComponent::CullAllRenderers(CameraComponent& Camera, Out std::stack<GameRendererComponent*>& VisibleRenderers)
{
	for (int Index = 0; Index < AllGameRendererComponents.size(); Index++)
	{
		GameRendererComponent* RendererComponent = AllGameRendererComponents[Index];
		if (RendererComponent->IsPendingDestroy() || !RendererComponent->IsEnabled())
		{
			continue;
		}
		if (RendererComponent->Cull(Camera))
		{
			RendererComponent->bWasCulled = true;
		}
		else
		{
			RendererComponent->bWasCulled = false;
			VisibleRenderers.push(RendererComponent);
		}
	}
}