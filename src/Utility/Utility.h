#pragma once
#include <string>
#include <glm/glm.hpp>

struct b2Vec2;

class Utility
{
public:
	static std::string ToString(const b2Vec2& Vector);
	static std::string ToString(const glm::vec4& Vector);
};