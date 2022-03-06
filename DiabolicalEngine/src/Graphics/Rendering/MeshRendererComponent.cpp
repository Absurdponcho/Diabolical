#include "MeshRendererComponent.h"
#include "Camera.h"
#include "Game/GameManager.h"
#include "MeshPrimitives.h"
#include "ECS/ECS.h"

flecs::query<DMeshRendererComponent, Transform3D> DMeshRendererComponent::RendererQuery;

void DMeshRendererComponent::InitECSSystems()
{
	
	//{	// Create Transform Query
	//	RendererQuery = DUtilityECS::GetECSWorld().query_builder<DMeshRendererComponent, Transform3D>()
	//		.term(flecs::Disabled)
	//		.oper(flecs::Not)
	//		.arg(3)
	//		.term<DCameraComponent>()
	//		.oper(flecs::Optional)
	//		.build();
	//}

	//{	// Create Scene Updater
	//	struct SceneUpdater {};
	//	DUtilityECS::GetECSWorld().entity("Scene Updater").add<SceneUpdater>();
	//	DUtilityECS::GetECSWorld().system<SceneUpdater>("Scene Updater System")
	//		.kind(flecs::PreStore)
	//		.each([](SceneUpdater& s)
	//	{
	//		RendererQuery.each(&RenderTransform3D);
	//	});
	//}


	DUtilityECS::GetECSWorld().system<DMeshRendererComponent, Transform3D>("Render")
		.kind(flecs::OnStore)
		.each(DMeshRendererComponent::RenderTransform3D);


}

void DMeshRendererComponent::RenderTransform3D(DMeshRendererComponent& Renderer, Transform3D& Transform)
{
	DMesh* Mesh = Renderer.Mesh.Get();
	if (!Mesh) return;

	DMaterialInstance* MaterialInstance = Renderer.MaterialInstance.Get();
	if (!MaterialInstance) return;

	DCameraComponent* CameraComponent = DCameraComponent::GetActiveCamera();
	Check(CameraComponent);
	if (!CameraComponent) return;

	Matrix4x4 ViewMatrix = CameraComponent->GetViewMatrix();
	Matrix4x4 ProjectionMatrix = CameraComponent->GetPerspectiveProjectionMatrix();

	Matrix4x4 ModelMatrix = Transform.GetModelMatrix();

	Matrix4x4 MVPMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;

	Check(MaterialInstance->SetUniform("MVP", MVPMatrix));

	MaterialInstance->Bind();
	Mesh->Draw();


	//LOG(DString::Format("%s -> %s", ParentTransform ? *ParentTransform->Name : "", Transform ? *Transform->Name : ""));
	
}

DEntity DMeshRendererComponent::MakeMeshRendererEntity()
{
	return DEntity();
}
