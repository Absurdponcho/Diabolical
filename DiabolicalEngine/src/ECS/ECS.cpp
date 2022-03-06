#include "ECS.h"
#include "Graphics/Rendering/MeshRendererComponent.h"
#include "Graphics/Rendering/Camera.h"
#include "Maths/Maths.h"
#include "Graphics/Rendering/Scene.h"

flecs::world DUtilityECS::ECSWorld;

void DUtilityECS::InitializeECSSystems()
{
	DScene::InitECSSystems();
	DMeshRendererComponent::InitECSSystems();
	DCameraComponent::InitECSSystems();
}

flecs::world& DUtilityECS::GetECSWorld()
{
	return ECSWorld;
}
