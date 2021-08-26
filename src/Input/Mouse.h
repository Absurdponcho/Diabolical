#pragma once
#include "..\GunGame.h"
#include <vector>
#include <glm/glm.hpp>
#include "Interfaces/IMouseListener.h"
class Mouse
{
public:
	const int N_MOUSEBUTTONS = 7;
	enum {
		BUTTON_LEFT = 0,
		BUTTON_RIGHT = 1,
		BUTTON_MIDDLE = 2,
		BUTTON_XBUTTON1 = 3,
		BUTTON_XBUTTON2 = 4,
		BUTTON_MWHEELUP = 5,
		BUTTON_MWHEELDOWN = 6
	};

	Mouse();
	virtual ~Mouse() {};

	void AttachMouseListener(IMouseListener* listener);
	void RemoveMouseListener(IMouseListener* listener);
	void OnNewFrame();
	
	virtual glm::vec2 GetMousePos() const;
	virtual bool IsButtonDown(unsigned int button)  const;
	virtual bool IsButtonUp(unsigned int button)  const;

private:
	std::vector<IMouseListener*> Listeners;
	std::vector<bool> ButtonsDown;
	std::vector<bool> ButtonsUp;
	glm::vec2 Pos;
};

