#include "ECS.h"
#include "Graphics/Rendering/MeshRendererComponent.h"
#include "Graphics/Rendering/Camera.h"

void DUtilityECS::InitializeECSSystems()
{
	DMeshRendererComponent::InitECSSystems();
	DCameraComponent::InitECSSystems();
}

flecs::world& DUtilityECS::GetECSWorld()
{
	return ECSWorld;
}
