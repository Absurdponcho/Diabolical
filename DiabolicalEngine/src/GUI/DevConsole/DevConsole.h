#pragma once
#include "../IWindow.h"
#include <array>
#include "../../Types/DString.h"

class DDevConsole : public IWindow
{
public:
	DDevConsole();
	virtual ~DDevConsole();
	void Draw() override;
	float Height = 260;
	DString Input;

	void ParseCommandQuotations(DVector<DString>& Params);

};

