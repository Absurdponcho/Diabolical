#pragma once
#include "..\ImGui\imgui.h"
#include <vector>
#include "../Types/DVector.h"

class IWindow
{
public:
	IWindow();
	virtual ~IWindow();
	bool operator==(const IWindow& RHS);
	virtual void Tick();
	virtual void Draw() = 0;
	virtual bool IsShown();
	virtual bool Enable();
	virtual bool Disable();
	virtual void Toggle();

	static DVector<IWindow*> WindowList;
private:
	size_t WindowID;
	bool bEnabled = false;
};

