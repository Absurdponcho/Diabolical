#pragma once

#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>

typedef glm::ivec2 IntVector2;
typedef glm::ivec3 IntVector3;
typedef glm::ivec4 IntVector4;

class Matrix4x4 : public glm::mat4x4
{
public:
	using glm::mat4x4::mat4x4;
	static const Matrix4x4 Identity;
};

class Matrix3x3 : public glm::mat3x3
{
public:
	using glm::mat3x3::mat3x3;
	static const Matrix3x3 Identity;
};

class Vector4 : public glm::vec4
{
public:
	using glm::vec4::vec4;
	static const Vector4 ZeroVector;
};

class Vector3 : public glm::vec3
{
public:
	using glm::vec3::vec3;
	static const Vector3 ZeroVector;
};

class Vector2 : public glm::vec2
{
public:
	using glm::vec2::vec2;
	static const Vector2 ZeroVector;
};

class Quaternion : public glm::quat
{
public:
	using glm::quat::quat;
	static const Quaternion Identity;
};

class Transform3D
{
public:
	inline Vector3 GetPosition() { return Position; };
	inline Vector3 GetScale() { return Scale; };
	inline Quaternion GetRotation() { return Rotation; };
	inline Vector3 GetEulerRotation() { return glm::eulerAngles(GetRotation()); };

	inline void SetPosition(Vector3 NewPosition) 
	{ 
		Position = NewPosition; 
	};

	inline void SetScale(Vector3 NewScale) 
	{ 
		Scale = NewScale; 
	};

	inline void SetEulerRotation(Vector3 NewEulerRotation) 
	{
		SetRotation(glm::quat(NewEulerRotation * 0.01745329251994329576923690768489f));
	}

	inline void SetRotation(Quaternion NewRotation) 
	{ 
		Rotation = NewRotation; 
	};

private:
	Vector3 Position = Vector3::ZeroVector;
	Vector3 Scale = Vector3(1, 1, 1);
	Quaternion Rotation = Quaternion::Identity;
};
