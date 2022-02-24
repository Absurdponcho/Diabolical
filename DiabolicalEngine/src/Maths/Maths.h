#pragma once

#include <glm/glm.hpp>
#include <glm/ext/quaternion_float.hpp>

typedef glm::ivec2 IntVector2;
typedef glm::ivec3 IntVector3;

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
	inline Vector3 GetScale() { return Position; };
	inline Quaternion GetRotation() { return Rotation; };

	inline void SetPosition(Vector3 NewPosition) 
	{ 
		Position = NewPosition; 
	};

	inline void SetScale(Vector3 NewScale) 
	{ 
		Scale = NewScale; 
	};

	inline void SetRotation(Quaternion NewRotation) 
	{ 
		Rotation = NewRotation; 
	};

private:
	Vector3 Position = Vector3::ZeroVector;
	Vector3 Scale = {1, 1, 1};
	Quaternion Rotation = Quaternion::Identity;
};
