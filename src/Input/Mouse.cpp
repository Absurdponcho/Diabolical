#include "Mouse.h"

Mouse::Mouse() : 
	Listeners(),
	ButtonsUp(N_MOUSEBUTTONS),
	ButtonsDown(N_MOUSEBUTTONS),
	Pos(0,0)
{
	for (size_t i = 0; i < N_MOUSEBUTTONS; i++) {
		ButtonsUp[i] = false;
		ButtonsDown[i] = false;
	}
}

void Mouse::AttachMouseListener(IMouseListener* listener)
{
	Check(listener);
	Listeners.push_back(listener);
}

void Mouse::RemoveMouseListener(IMouseListener* listener)
{
	Check(listener);
	std::vector<IMouseListener*>::iterator iter;
	iter = std::find(Listeners.begin(), Listeners.end(), listener);
	if (iter != Listeners.end()) {
		Listeners.erase(iter);
	}
}

void Mouse::OnNewFrame()
{
	// destroy old state on new frame
	for (size_t i = 0; i < N_MOUSEBUTTONS; i++) {
		ButtonsUp[i] = false;
		ButtonsDown[i] = false;
	}

}

glm::vec2 Mouse::GetMousePos()  const
{
	return Pos;
}

bool Mouse::IsButtonDown(unsigned int button)  const
{
	if (button < ButtonsDown.size()) {
		return ButtonsDown[button];
	}
	return false;
}

bool Mouse::IsButtonUp(unsigned int button)  const
{
	if (button < ButtonsUp.size()) {
		return ButtonsDown[button];
	}
	return false;
}
