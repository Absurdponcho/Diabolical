#include "DevConsole.h"
#include "Graphics/WindowManager.h"
#include "Types/DString.h"
#include "Logging/Logging.h"
#include "ImGui/imgui_stdlib.h"
#include "ImGui/imgui.h"

DDevConsole::DDevConsole()
{

}

DDevConsole::~DDevConsole()
{
	
}

void DDevConsole::Draw()
{
	int FrameId = 1;

	ImGui::SetNextWindowPos({0, 0});
	ImGui::SetNextWindowSize({(float)DWindowManager::Get().GetScreenSize().x, Height});
	ImGui::SetNextWindowSizeConstraints({(float)DWindowManager::Get().GetScreenSize().x, 128.f},{(float)DWindowManager::Get().GetScreenSize().x, 768.f});

	ImGuiStyle OldStyle = ImGui::GetStyle();

	ImGui::GetStyle().WindowPadding = {0,0};

	ImGui::Begin("DevConsole", NULL, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar);
	{
		Height = ImGui::GetWindowSize().y;
		ImGui::Text("Development Console");

		ImGui::BeginChildFrame(FrameId++, {(float)DWindowManager::Get().GetScreenSize().x, Height - 40});
		{
			ImGui::Text(*Logging::GetLogRecap());

			ImGui::SetScrollHereY(1);
		}
		ImGui::EndChildFrame();

		ImGui::SetNextItemWidth(420);
		if (ImGui::InputText("DevInput", (std::string*)&Input, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			Logging::LogPlain(DString("CMD > ").Append(Input).Append("\n"), 15);
			Input = "";
			ImGui::SetKeyboardFocusHere(-1);
		}

	}
	ImGui::End();

	ImGui::GetStyle() = OldStyle;
}


void DDevConsole::RegisterCommand(DString Command, DAction<DVector<DString>> function)
{
	if (Commands.find(Command) == Commands.end())
	{
//		Commands.insert(Commands.end(), std::pair(Command, function));
		Logging::LogPlain(DString("DevConsole - Added Command").Append(Command), 15);
	}
}

void DDevConsole::ParseCommand(DString Input)
{
	if (Input.Length())
	{
		DString command, params;
		Input.Split(" ", command, params);
		DVector<DString> params_v = params.SplitAll(" ,");
		auto cmd = Commands.find(command);
		if (cmd != Commands.end()) {
			cmd->second.Invoke(params_v);
		}
	}
}

void DDevConsole::ParseCommandQuotations(DVector<DString>& Params)
{
	DVector<DString> RetVal;

	bool bInQuotation = false;
	DString CurrentBuiltString;

	for (DString& Param : Params)
	{
		if (bInQuotation)
		{
			if (Param.EndsWith("\""))
			{
				bInQuotation = false;
				CurrentBuiltString.Append(Param.RemoveEnd(1));
				if (CurrentBuiltString.EndsWith(" "))
				{
					CurrentBuiltString = CurrentBuiltString.RemoveEnd(1);
				}
				RetVal.PushBack(CurrentBuiltString);
				CurrentBuiltString = "";
			}
			else
			{
				CurrentBuiltString.Append(Param);
				CurrentBuiltString.Append(" ");
			}
		}
		else
		{
			if (Param.StartsWith("\""))
			{
				CurrentBuiltString.Append(Param.RemoveStart(1));
				CurrentBuiltString.Append(" ");
				bInQuotation = true;
			}
			else
			{
				RetVal.PushBack(Param);
			}
		}
	}

	Params = RetVal;
}