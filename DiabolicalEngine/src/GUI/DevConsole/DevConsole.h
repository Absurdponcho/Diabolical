#pragma once
#include "GUI/IWindow.h"
#include <array>
#include "Types/DString.h"
#include "Types/Action.h"
#include <map>

class DDevConsole : public IWindow
{
public:

	DDevConsole();
	virtual ~DDevConsole();
	void Draw() override;
	float Height = 260;
	DString Input;
	std::map<DString, DAction<DVector<DString>>> Commands;
	void RegisterCommand(DString Command, DAction<DVector<DString>> function);
	void UnregisterCommand(DString Command);
	void ParseCommand(DString CommandString);
	void ParseCommandQuotations(DVector<DString>& Params);
	void ExecuteCommand(DString Command, DVector<DString>& Params);

};

