#include "MeshRendererComponent.h"
#include "Camera.h"
#include "Game/GameManager.h"
#include "MeshPrimitives.h"
#include "ECS/ECS.h"

void DMeshRendererComponent::InitECSSystems()
{
	
	DUtilityECS::GetECSWorld().system<DMeshRendererComponent, Transform3D>("Render")
		.kind(flecs::OnStore)
		.each(DMeshRendererComponent::RenderTransform3D);
}

void DMeshRendererComponent::RenderTransform3D(const flecs::entity& ent, DMeshRendererComponent& Renderer, Transform3D& Transform)
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

	Matrix4x4 ModelMatrix = glm::mat4(1.f);
	ModelMatrix = glm::translate(ModelMatrix, Transform.GetPosition() + Vector3(0, sin(DGameManager::Get().GetGameTime() * 2 + ent.raw_id()), 0));
	ModelMatrix = glm::scale(ModelMatrix, Transform.GetScale());
	ModelMatrix = glm::rotate(ModelMatrix, 0.f, Vector3(0, 1, 0));

	Matrix4x4 MVPMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;

	Check(MaterialInstance->SetUniform("MVP", MVPMatrix));

	MaterialInstance->Bind();
	Mesh->Draw();
}

flecs::entity DMeshRendererComponent::MakeMeshRendererEntity()
{
	return flecs::entity();
}
