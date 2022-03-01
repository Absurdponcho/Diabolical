#pragma once
#include "Types/DMemory.h"
#include "Mesh.h"
#include "MaterialInstance.h"
#include "Maths/Maths.h"
#include "ECS/ECS.h"

class DMeshRendererComponent
{
public:
	DMeshRendererComponent() {};

	DMeshRendererComponent(const DMeshRendererComponent& Other)
		: Mesh(Other.Mesh)
	{
		MaterialInstance = std::make_unique<DMaterialInstance>(*Other.MaterialInstance.Get());
	}

	DMeshRendererComponent(DSharedPtr<DMesh> NewMesh, DSharedPtr<DMaterial> NewMaterial)
		: Mesh(NewMesh), MaterialInstance(std::make_unique<DMaterialInstance>(NewMaterial)) {}

	DMeshRendererComponent& operator= (DMeshRendererComponent&& Other)
	{
		Mesh = Other.Mesh;
		MaterialInstance = std::make_unique<DMaterialInstance>(*Other.MaterialInstance.Get());
		return *this;
	}

	DMeshRendererComponent& operator= (const DMeshRendererComponent& Other)
	{
		Mesh = Other.Mesh;
		MaterialInstance = std::make_unique<DMaterialInstance>(*Other.MaterialInstance.Get());
		return *this;
	}

	DSharedPtr<DMesh> Mesh;
	DUniquePtr<DMaterialInstance> MaterialInstance;

	static void InitECSSystems();
	static void RenderTransform3D(const flecs::entity& ent, DMeshRendererComponent& Renderer, Transform3D& Transform);
	static flecs::entity MakeMeshRendererEntity();

protected:

	static flecs::entity TestRenderEntity;
};

