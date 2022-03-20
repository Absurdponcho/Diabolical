#pragma once
#include "Maths/Maths.h"
#include "ECS/ECS.h"
#include "Check.h"

class DCameraComponent
{
public:

	const Matrix4x4& GetPerspectiveVPMatrix();
	const Matrix4x4& GetViewMatrix();
	const Matrix4x4& GetPerspectiveProjectionMatrix();

	float GetAspectRatio();

	static DCameraComponent* GetActiveCamera();

	static void SetActiveCamera(DEntity& CameraEntity);

	static void RemoveActiveCamera();

	void SetParentEntity(DEntity NewParent)
	{
		Check (NewParent.IsAlive());
		ParentEntity = NewParent;
		Check(ParentEntity.IsAlive());
	}

	static void InitECSSystems();
	DEntity ParentEntity;

protected:
	static DEntity ActiveCameraComponent;


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

