#include "Camera.h"
#include "Graphics/WindowManager.h"
#include "Game/GameManager.h"
#include "ECS/ECS.h"

DEntity DCameraComponent::ActiveCameraComponent;

const Matrix4x4& DCameraComponent::GetPerspectiveVPMatrix()
{
	Check(ParentEntity.IsAlive());

	Transform3D* ThisTransform = (Transform3D*)ParentEntity.GetComponent<Transform3D>();
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
	Transform3D* ThisTransform = (Transform3D*)ParentEntity.GetComponent<Transform3D>();
	Check(ThisTransform);

	bPerspectiveProjectionMatrixDirty = true;
	bPerspectiveVPMatrixDirty = true;
	bViewMatrixDirty = true;

	LastCleanPosition = ThisTransform->GetPosition();
	LastCleanRotation = ThisTransform->GetRotation();
}

DCameraComponent* DCameraComponent::GetActiveCamera()
{
	if (!ActiveCameraComponent.IsAlive()) return nullptr;
	return (DCameraComponent*)ActiveCameraComponent.GetComponent<DCameraComponent>();
}

void DCameraComponent::SetActiveCamera(DEntity& CameraEntity)
{
	Check(CameraEntity.IsAlive());
	if (!CameraEntity.IsAlive()) return;

	DCameraComponent* CameraComponent = CameraEntity.GetComponentMutable<DCameraComponent>();
	Check(CameraComponent);
	if (!CameraComponent) return;
	CameraComponent->ParentEntity = CameraEntity;

	const Transform3D* Transform = CameraEntity.GetComponent<Transform3D>();
	Check(Transform);
	if (!Transform) return;

	ActiveCameraComponent = CameraEntity;
}

void DCameraComponent::RemoveActiveCamera()
{
	ActiveCameraComponent = DEntity();
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
		Check(ParentEntity.IsAlive());
		Transform3D* ThisTransform = (Transform3D*)ParentEntity.GetComponent<Transform3D>();
		Check(ThisTransform);

		Vector3 EulerRotation = ThisTransform->GetEulerRotation();
		//FPS camera:  RotationX(pitch) * RotationY(yaw)
		glm::quat qPitch = glm::angleAxis(EulerRotation.x, glm::vec3(1, 0, 0));
		glm::quat qYaw = glm::angleAxis(EulerRotation.y, glm::vec3(0, 1, 0));
		glm::quat qRoll = glm::angleAxis(EulerRotation.z, glm::vec3(0, 0, 1));

		//For a FPS camera we can omit roll
		glm::quat orientation = qPitch * qYaw * qRoll;
		orientation = glm::normalize(orientation);
		glm::mat4 rotate = glm::mat4_cast(orientation);

		glm::mat4 translate = glm::mat4(1.0f);
		translate = glm::translate(translate, -ThisTransform->GetPosition());

		ViewMatrix = rotate * translate;

		bViewMatrixDirty = false;
	}

	return ViewMatrix;
}

const Matrix4x4& DCameraComponent::GetPerspectiveProjectionMatrix()
{
	if (bPerspectiveProjectionMatrixDirty)
	{
		Transform3D* ThisTransform = (Transform3D*)ParentEntity.GetComponent<Transform3D>();
		Check(ThisTransform);

		float AspectRatio = GetAspectRatio();
		PerspectiveProjectionMatrix = glm::perspective(glm::radians(90.f), AspectRatio, 0.001f, 1000.f);

		bPerspectiveProjectionMatrixDirty = false;
	}

	return PerspectiveProjectionMatrix;
}

void DCameraComponent::InitECSSystems()
{
	/*DUtilityECS::GetECSWorld().system<DCameraComponent>("Camera Initialize")
		.kind(flecs::OnSet)
		.kind(flecs::OnAdd)
		.each([](const DEntity& ent, DCameraComponent& Camera)
	{
		Check(ent.IsAlive());
		Camera.SetParentEntity(ent);
	});*/
}
