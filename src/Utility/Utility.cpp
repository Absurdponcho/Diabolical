#include "Utility.h"
#include <box2d/b2_math.h>
#include <sstream>

std::string Utility::ToString(const b2Vec2& Vector)
{
	std::ostringstream StringStream;
	StringStream << "( " << Vector.x << " , " << Vector.y << " )";
	return StringStream.str();
}