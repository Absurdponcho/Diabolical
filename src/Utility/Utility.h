#pragma once
#include <string>
#include <glm/glm.hpp>
#include <vector>

class CameraComponent;
struct b2Vec2;

class Utility
{
public:
	static std::string ToString(const b2Vec2& Vector);
	static std::string ToString(const glm::vec4& Vector);
	static std::string ToString(const glm::vec3& Vector);
	static void DrawDebugLine(const glm::vec3& From, const glm::vec3& To);
	static std::wstring StringToWString(const std::string& String);
	static std::string WStringToString(const std::wstring& String);
	static glm::vec3 ScreenToWorld(glm::vec2 ScreenPos, CameraComponent* Camera);

	// In radians
	static float AngleBetween(glm::vec2 From, glm::vec2 To);

	template<class T, class Pred>
	static T FindPred(std::vector<T> Vector, Pred Predicate)
	{
		for (int Index = 0; Index < Vector.size(); Index++)
		{
			T Value = Vector[Index];
			if (Predicate(Value)) return Value;
		}
		return nullptr;
	}

	template<class T>
	static bool Find(std::vector<T> Vector, T Value)
	{
		for (int Index = 0; Index < Vector.size(); Index++)
		{
			if (Value == Vector[Index]) return true;
		}
		return false;
	}

	
};