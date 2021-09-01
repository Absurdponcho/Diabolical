#include "InputManager.h"
#include "../Logging/Logging.h"
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include "../GunGame.h"
#include "../Utility/Utility.h"

std::unordered_map<SDL_Keycode, bool> InputManager::KeyInputMap;
std::vector<InputManager::KeyMapping*> InputManager::KeyMappings;

std::unordered_map<Uint8, bool> InputManager::MouseButtonInputMap;
std::vector<InputManager::MouseButtonMapping*> InputManager::MouseButtonMappings;

std::vector<InputManager::ActionGroup*> InputManager::ActionGroups;
std::vector<InputDelegate> InputManager::InputDelegates;

InputManager::ActionGroup::ActionGroup(std::string Action)
	:	ActionName(Action)
{
	LOG("ActionGroup::ActionGroup()", "Added new Action Group \"" + Action + "\"");

}

void InputManager::ActionGroup::BindFunction(std::function<void (ActionInfo&)> Function)
{
	LOGVERBOSE("ActionGroup::ActionGroup()", "Bound function to \"" + GetActionName() + "\".");
	FunctionBindings.push_back(Function);
}

void InputManager::ActionGroup::BindInputDelegate(InputDelegate Delegate)
{
	//LOGVERBOSE("ActionGroup::ActionGroup()", "Bound delegate to \"" + GetActionName() + "\" for UID (" + std::to_string(Delegate) + ")");
	InputDelegateBindings.push_back(Delegate);
}

const std::string& InputManager::ActionGroup::GetActionName()
{
	return ActionName;
}

void InputManager::ActionGroup::Execute(ActionInfo& Info)
{
	for (std::function<void(ActionInfo&)>& Function : FunctionBindings)
	{
		Function(Info);
	}

	for (int Index = (int)InputDelegateBindings.size() - 1; Index >= 0; Index --)
	{
		InputDelegateBindings[Index].Execute(Info);
	}
}

SDL_Keycode InputManager::KeyMapping::GetSymbol()
{
	return KeySymbol;
}

void InputManager::KeyMapping::AddMapping(std::string ActionName)
{
	if (!Utility::Find(ActionMappings, ActionName))
	{
		LOG("ActionGroup::AddMapping()", "Added ActionName \"" + ActionName + "\" to key \"" + std::string(SDL_GetKeyName(GetSymbol())) + "\".");

		ActionMappings.push_back(ActionName);
	}
	else
	{
		LOGWARNING("ActionGroup::AddMapping()", "Attempted to add KeyMapping \"" + std::string(SDL_GetKeyName(GetSymbol())) + "\" to \"" + ActionName + "\" which already exists!");
	}
}

InputManager::KeyMapping::KeyMapping(SDL_Keycode Symbol)
	: KeySymbol(Symbol)
{
	LOG("KeyMapping::KeyMapping()", "Added new Keymap \"" + std::string(SDL_GetKeyName(Symbol)) + "\"");
}

const std::vector<std::string>& InputManager::KeyMapping::GetActionMappings()
{
	return ActionMappings;
}

void InputManager::HandleKeyboardEvent(SDL_KeyboardEvent& KeyboardEvent)
{
	switch (KeyboardEvent.state)
	{
	case SDL_PRESSED:
		PushAction(KeyboardEvent);
		KeyInputMap[KeyboardEvent.keysym.sym] = true;

		break;
	case SDL_RELEASED:
		PushAction(KeyboardEvent);
		KeyInputMap[KeyboardEvent.keysym.sym] = false;

		break;
	default:
		Check(false); // what
	}
	
}

void InputManager::HandleMouseWheelEvent(SDL_MouseWheelEvent& MouseWheelEvent)
{
	if (MouseWheelEvent.y > 0) {

	}
	else {

	}
}

void InputManager::HandleMouseButtonEvent(SDL_MouseButtonEvent& MouseEvent)
{
	switch (MouseEvent.state)
	{
	case SDL_PRESSED:
		PushActionMouseButton(MouseEvent);
		MouseButtonInputMap[MouseEvent.button] = true;

		break;
	case SDL_RELEASED:
		PushActionMouseButton(MouseEvent);
		MouseButtonInputMap[MouseEvent.button] = false;
		break;
	default:
		Check(false); // what
	}
}

void InputManager::PushAction(SDL_KeyboardEvent KBEvent)
{
	KeyMapping* Keymap = nullptr;
	if (Keymap = Utility::FindPred(KeyMappings, [=](KeyMapping* RHS) { return RHS->GetSymbol() == KBEvent.keysym.sym; }))
	{
		for (const std::string& ActionName : Keymap->GetActionMappings())
		{
			if (ActionGroup* Group = Utility::FindPred(ActionGroups, [=](ActionGroup* RHS) { return RHS->GetActionName() == ActionName; }))
			{
				LOGVERBOSE("InputManager::PushAction()", "Action \"" + ActionName + "\" invoked.");
				ActionInfo Info = ActionInfo();
				switch (KBEvent.state)
				{
				case SDL_PRESSED:
					if (KeyInputMap[KBEvent.keysym.sym])
					{
						Info.InputType = EInputType::IT_Held;
					}
					else
					{
						Info.InputType = EInputType::IT_Pressed;
					}
					break;
				case SDL_RELEASED:
					Info.InputType = EInputType::IT_Released;
					break;
				default:
					Check(false);
				}
				Info.KeyboardEvent = &KBEvent;
				Group->Execute(Info);
			}
		}
	}
	else
	{
		//LOGVERBOSE("InputManager::PushAction()", "Tried to push actions for key but there was no mapping.");
	}
}

void InputManager::PushActionMouseButton(SDL_MouseButtonEvent MSBEvent)
{
	MouseButtonMapping* MouseMap = nullptr;
	if (MouseMap = Utility::FindPred(MouseButtonMappings, [=](MouseButtonMapping* RHS) { return RHS->GetButton() == MSBEvent.button; }))
	{
		for (const std::string& ActionName : MouseMap->GetActionMappings())
		{
			if (ActionGroup* Group = Utility::FindPred(ActionGroups, [=](ActionGroup* RHS) { return RHS->GetActionName() == ActionName; }))
			{
				LOGVERBOSE("InputManager::PushAction()", "Action \"" + ActionName + "\" invoked.");
				ActionInfo Info = ActionInfo();
				switch (MSBEvent.state)
				{
				case SDL_PRESSED:
					if (MouseButtonInputMap[MSBEvent.button])
					{
						Info.InputType = EInputType::IT_Held;
					}
					else
					{
						Info.InputType = EInputType::IT_Pressed;
					}
					break;
				case SDL_RELEASED:
					Info.InputType = EInputType::IT_Released;
					break;
				default:
					Check(false);
				}
				Info.MouseButtonEvent = &MSBEvent;
				Group->Execute(Info);
			}
		}
	}
	else
	{
		//LOGVERBOSE("InputManager::PushAction()", "Tried to push actions for key but there was no mapping.");
	}
}

bool InputManager::IsKeyHeld(SDL_Keycode Symbol)
{
	return KeyInputMap[Symbol];
}

bool InputManager::IsMouseButtonHeld(Uint8 Button)
{
	return MouseButtonInputMap[Button];
}

void InputManager::AddKeyMapping(std::string Action, SDL_Keycode Symbol)
{
	// Add key mapping ==========================
	KeyMapping* KeyMap = nullptr;
	if (!(KeyMap = Utility::FindPred(KeyMappings, [=](KeyMapping* RHS) {return RHS->GetSymbol() == Symbol; })))
	{
		KeyMap = new KeyMapping(Symbol);
		KeyMappings.push_back(KeyMap);
	}
	Check(KeyMap);

	KeyMap->AddMapping(Action);
	// ==========================================


	// Add action group =========================
	ActionGroup* Group = nullptr;
	if (!(Group = Utility::FindPred(ActionGroups, [=](ActionGroup* RHS) {return RHS->GetActionName() == Action; })))
	{
		Group = new ActionGroup(Action);
		ActionGroups.push_back(Group);
	}
	Check(Group);
	// ==========================================

}

void InputManager::AddMouseButtonMapping(std::string Action, Uint8 Button)
{
	// Add mouse button mapping ==========================
	MouseButtonMapping* MBMap = nullptr;
	if (!(MBMap = Utility::FindPred(MouseButtonMappings, [=](MouseButtonMapping* RHS) {return RHS->GetButton() == Button; }))) {
		MBMap = new MouseButtonMapping(Button);
		MouseButtonMappings.push_back(MBMap);
	}
	Check(MBMap);

	MBMap->AddMapping(Action);
	// ==========================================
	// Add action group =========================
	ActionGroup* Group = nullptr;
	if (!(Group = Utility::FindPred(ActionGroups, [=](ActionGroup* RHS) {return RHS->GetActionName() == Action; })))
	{
		Group = new ActionGroup(Action);
		ActionGroups.push_back(Group);
	}
	Check(Group);
	// ==========================================
}

void InputManager::AddMouseWheelMapping(std::string Action, Uint32 Direction)
{

}

InputManager::MouseButtonMapping::MouseButtonMapping(Uint8 Button) :
	Button(Button)
{
}

Uint8 InputManager::MouseButtonMapping::GetButton()
{
	return Button;
}

void InputManager::MouseButtonMapping::AddMapping(std::string ActionName)
{
	if (!Utility::Find(ActionMappings, ActionName))
	{
		LOG("ActionGroup::AddMapping()", "Added ActionName \"" + ActionName + "\" to Mouse");

		ActionMappings.push_back(ActionName);
	}
	else
	{
		LOGWARNING("ActionGroup::AddMapping()", "Attempted to add a mouse button to \"" + ActionName + "\" which already exists!");
	}
}

const std::vector<std::string>& InputManager::MouseButtonMapping::GetActionMappings()
{
	return ActionMappings;
}
