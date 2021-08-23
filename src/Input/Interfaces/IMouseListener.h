#pragma once
#include <glm/glm.hpp>
class IMouseListener
{
public:
	IMouseListener() {};
	virtual ~IMouseListener() {};
	virtual void MouseMove(glm::vec2& pos) {};
	virtual void MouseButtonPress(glm::vec2& pos, int button) {};
	virtual void MouseButtonRelease(glm::vec2& pos, int button) {};
};

