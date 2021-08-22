#include "Utility.h"
#include <box2d/b2_math.h>
#include <sstream>

std::string Utility::ToString(const b2Vec2& Vector)
{
	std::ostringstream StringStream;
	StringStream << "( " << Vector.x << " , " << Vector.y << " )";
	return StringStream.str();
}

std::string Utility::ToString(const glm::vec4& Vector)
{
	std::ostringstream StringStream;
	StringStream << "( " << Vector.x << " , " << Vector.y << " , " << Vector.z << " , " << Vector.w << " )";
	return StringStream.str();
}

std::string Utility::ToString(const glm::vec3& Vector)
{
	std::ostringstream StringStream;
	StringStream << "( " << Vector.x << " , " << Vector.y << " , " << Vector.z << " )";
	return StringStream.str();
}