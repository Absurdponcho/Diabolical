#include "Camera.h"
#include "Graphics/WindowManager.h"

flecs::entity_view DCameraComponent::ActiveCameraComponent;

const Matrix4x4& DCameraComponent::GetPerspectiveVPMatrix()
{
	Check(ParentEntity.is_alive());

	Transform3D* ThisTransform = (Transform3D*)ParentEntity.get<Transform3D>();
	Check(ThisTransform);

	if (ThisTransform->GetPosition() != LastCleanPosition || ThisTransform->GetRotation() != LastCleanRotation)
	{
		DirtyAll();
	}

	if (bPerspectiveVPMatrixDirty)
	{
		PerspectiveProjectionMatrix = GetPerspectiveProjectionMatrix() * GetViewMatrix();
		bPerspectiveVPMatrixDirty = false;
	}

	return PerspectiveProjectionMatrix;
}

void DCameraComponent::DirtyAll()
{
	Transform3D* ThisTransform = (Transform3D*)ParentEntity.get<Transform3D>();
	Check(ThisTransform);

	bPerspectiveProjectionMatrixDirty = true;
	bPerspectiveVPMatrixDirty = true;
	bViewMatrixDirty = true;

	LastCleanPosition = ThisTransform->GetPosition();
	LastCleanRotation = ThisTransform->GetRotation();
}

DCameraComponent* DCameraComponent::GetActiveCamera()
{
	if (!ActiveCameraComponent.is_alive()) return nullptr;
	return (DCameraComponent*)ActiveCameraComponent.get<DCameraComponent>();
}

void DCameraComponent::SetActiveCamera(const flecs::entity& CameraEntity)
{
	Check(CameraEntity.is_alive());
	if (!CameraEntity.is_alive()) return;

	const DCameraComponent* CameraComponent = CameraEntity.get<DCameraComponent>();
	Check(CameraComponent);
	if (CameraComponent)
	{
		ActiveCameraComponent = CameraEntity;
	}
}

void DCameraComponent::RemoveActiveCamera()
{
	ActiveCameraComponent = flecs::entity_view();
}

float DCameraComponent::GetAspectRatio()
{
	Vector2 ScreenSize = DWindowManager::Get().GetScreenSize();
	return (float)ScreenSize.x / (float)ScreenSize.y;
}

const Matrix4x4& DCameraComponent::GetViewMatrix()
{
	if (bViewMatrixDirty)
	{
		Transform3D* ThisTransform = (Transform3D*)ParentEntity.get<Transform3D>();
		Check(ThisTransform);

		Vector3 ForwardVector = Vector3(0, 0, -1) * ThisTransform->GetRotation();
		Vector3 UpVector = Vector3(0, 1, 0) * ThisTransform->GetRotation();
		ViewMatrix = glm::lookAt(ThisTransform->GetPosition(), ThisTransform->GetPosition() + ForwardVector, UpVector);

		bViewMatrixDirty = false;
	}

	return ViewMatrix;
}

const Matrix4x4& DCameraComponent::GetPerspectiveProjectionMatrix()
{
	if (bPerspectiveProjectionMatrixDirty)
	{
		Transform3D* ThisTransform = (Transform3D*)ParentEntity.get<Transform3D>();
		Check(ThisTransform);

		float AspectRatio = GetAspectRatio();
		PerspectiveProjectionMatrix = glm::perspective(50.f, AspectRatio, 0.001f, 1000.f);

		bPerspectiveProjectionMatrixDirty = false;
	}

	return PerspectiveProjectionMatrix;
}
