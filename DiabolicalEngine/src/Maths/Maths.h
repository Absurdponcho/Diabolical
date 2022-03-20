#pragma once

#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Types/DString.h"
#include "ECS/ECS.h"

typedef glm::ivec2 IntVector2;
typedef glm::ivec3 IntVector3;
typedef glm::ivec4 IntVector4;

class Matrix4x4 : public glm::mat4x4
{
public:
	
	Matrix4x4() {}
	Matrix4x4(glm::mat4x4 In) : glm::mat4x4(In) {}

	static const Matrix4x4 Identity;
};

class Matrix3x3 : public glm::mat3x3
{
public:
	Matrix3x3() {}
	Matrix3x3(glm::mat3x3 In) : glm::mat3x3(In) {}

	static const Matrix3x3 Identity;
};

class Vector4 : public glm::vec4
{
public:
	Vector4() {}
	Vector4(glm::vec4 In) : glm::vec4(In) {}

	static const Vector4 ZeroVector;
};

class Vector3 : public glm::vec3
{
public:
	Vector3() {}
	Vector3(glm::vec3 In) : glm::vec3(In) {}

	template <typename X, typename Y, typename Z>
	Vector3(X x, Y y, Z z) : glm::vec3(x,y,z) {}

	static const Vector3 ZeroVector;
};

class Vector2 : public glm::vec2
{
public:
	Vector2() {}
	Vector2(glm::vec2 In) : glm::vec2(In) {}
	Vector2(glm::ivec2 In) : glm::vec2(In) {}

	template <typename X, typename Y>
	Vector2(X x, Y y) : glm::vec2(x, y) {}

	static const Vector2 ZeroVector;
};

class Quaternion : public glm::quat
{
public:
	Quaternion() {}
	Quaternion(glm::quat In) : glm::quat(In) {}

	static const Quaternion Identity;
};

class Transform3D
{
public:
	DString Name;

	Transform3D() {}

	Transform3D(Vector3 NewPosition, Vector3 NewScale, Quaternion NewRotation)
		: Position(NewPosition), Scale(NewScale), Rotation(NewRotation) {}

	Transform3D(Vector3 NewPosition, Vector3 NewScale, Vector3 NewEulerRotation)
		: Position(NewPosition), Scale(NewScale)
	{
		SetEulerRotation(NewEulerRotation);
	}

	inline Vector3 GetPosition() { return Position; };
	inline Vector3 GetScale() { return Scale; };
	inline Quaternion GetRotation() { return Rotation; };
	inline Vector3 GetEulerRotation() { return glm::eulerAngles(GetRotation()); };

	inline void SetPosition(Vector3 NewPosition) 
	{ 
		bMatrixDirty = true;
		Position = NewPosition;
	};

	inline void SetScale(Vector3 NewScale) 
	{ 
		bMatrixDirty = true;
		Scale = NewScale;
	};

	inline void SetEulerRotation(Vector3 NewEulerRotation) 
	{
		SetRotation(glm::quat(NewEulerRotation * 0.01745329251994329576923690768489f));
	}

	inline void SetRotation(Quaternion NewRotation) 
	{ 
		bMatrixDirty = true;
		Rotation = NewRotation; 
	};

	Matrix4x4 GetWorldSpaceModelMatrix()
	{
		return Matrix4x4();
	}

	Matrix4x4 GetModelMatrix()
	{
		if (bMatrixDirty)
		{
			ModelMatrix = Matrix4x4::Identity;
			ModelMatrix = glm::translate(ModelMatrix, GetPosition());
			ModelMatrix = glm::scale(ModelMatrix, GetScale());
			ModelMatrix *= glm::mat4_cast(GetRotation());
		}
		return ModelMatrix;
	}

	DEntity ThisEntity;
private:
	bool bMatrixDirty = true;
	Vector3 Position = Vector3::ZeroVector;
	Vector3 Scale = Vector3(1, 1, 1);
	Quaternion Rotation = Quaternion::Identity;
	Matrix4x4 ModelMatrix = glm::mat4(1.f);
};
