#pragma once
class Mouse
{
public:
	Mouse();
	const int nMouseButtons = 7;
	enum {
		BUTTON_LEFT = 0,
		BUTTON_RIGHT = 1,
		BUTTON_MIDDLE = 2,
		BUTTON_MWHEELUP = 3,
		BUTTON_MWHEELDOWN = 4,
		BUTTON_XBUTTON1 = 5,
		BUTTON_XBUTTON2 = 6
	};
private:

};

