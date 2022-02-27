#pragma once
#include "Maths/Maths.h"
#include "ECS/flecs.h"
#include "Check.h"

class DCameraComponent
{
public:

	const Matrix4x4& GetPerspectiveVPMatrix();
	const Matrix4x4& GetViewMatrix();
	const Matrix4x4& GetPerspectiveProjectionMatrix();

	float GetAspectRatio();

	static DCameraComponent* GetActiveCamera();

	static void SetActiveCamera(const flecs::entity& CameraEntity);

	static void RemoveActiveCamera();

	void SetParentEntity(flecs::entity NewParent)
	{
		ParentEntity = NewParent;
	}

protected:
	static flecs::entity_view ActiveCameraComponent;

	flecs::entity ParentEntity;

	Vector3 LastCleanPosition = Vector3::ZeroVector;
	Quaternion LastCleanRotation = Quaternion::Identity;

	void DirtyAll();

	bool bPerspectiveVPMatrixDirty = true;
	Matrix4x4 PerspectiveVPMatrix = Matrix4x4::Identity;
	bool bViewMatrixDirty = true;
	Matrix4x4 ViewMatrix = Matrix4x4::Identity;
	bool bPerspectiveProjectionMatrixDirty = true;
	Matrix4x4 PerspectiveProjectionMatrix = Matrix4x4::Identity;

};

