#include "InputManager.h"
#include "../Logging/Logging.h"
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include "../GunGame.h"
#include "../Utility/Utility.h"

std::unordered_map<SDL_Keycode, bool> InputManager::InputMap;
std::vector<InputManager::ActionGroup*> InputManager::ActionGroups;
std::vector<InputManager::KeyMapping*> InputManager::KeyMappings;
std::vector<InputDelegate> InputManager::InputDelegates;

InputManager::ActionGroup::ActionGroup(std::string Action)
	:	ActionName(Action)
{
	Logging::Log("ActionGroup::ActionGroup()", "Added new Action Group \"" + Action + "\"");

}

void InputManager::ActionGroup::BindFunction(std::function<void (ActionInfo&)> Function)
{
	Logging::LogVerbose("ActionGroup::ActionGroup()", "Bound function to \"" + GetActionName() + "\".");
	FunctionBindings.push_back(Function);
}

void InputManager::ActionGroup::BindInputDelegate(InputDelegate Delegate)
{
	//Logging::LogVerbose("ActionGroup::ActionGroup()", "Bound delegate to \"" + GetActionName() + "\" for UID (" + std::to_string(Delegate) + ")");
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
		Logging::Log("ActionGroup::AddMapping()", "Added ActionName \"" + ActionName + "\" to key \"" + std::string(SDL_GetKeyName(GetSymbol())) + "\".");

		ActionMappings.push_back(ActionName);
	}
	else
	{
		Logging::LogWarning("ActionGroup::AddMapping()", "Attempted to add KeyMapping \"" + std::string(SDL_GetKeyName(GetSymbol())) + "\" to \"" + ActionName + "\" which already exists!");
	}
}

InputManager::KeyMapping::KeyMapping(SDL_Keycode Symbol)
	: KeySymbol(Symbol)
{
	Logging::Log("KeyMapping::KeyMapping()", "Added new Keymap \"" + std::string(SDL_GetKeyName(Symbol)) + "\"");
}

const std::vector<std::string>& InputManager::KeyMapping::GetActionMappings()
{
	return ActionMappings;
}

void InputManager::BindFunction(std::string Action, std::function<void(ActionInfo&)> Function)
{
	InputManager::ActionGroup* Group = nullptr;
	if (!(Group = Utility::FindPred(ActionGroups, [=](InputManager::ActionGroup* RHS) {return RHS->GetActionName() == Action; })))
	{
		Group = new InputManager::ActionGroup(Action);
		ActionGroups.push_back(Group);
	}
	Group->BindFunction(Function);
}

void InputManager::HandleKeyboardEvent(SDL_KeyboardEvent& KeyboardEvent)
{
	switch (KeyboardEvent.state)
	{
	case SDL_PRESSED:
		if (!InputMap[KeyboardEvent.keysym.sym])
		{
			PushAction(KeyboardEvent.keysym);
		}
		InputMap[KeyboardEvent.keysym.sym] = true;

		break;
	case SDL_RELEASED:
		PushAction(KeyboardEvent.keysym);
		InputMap[KeyboardEvent.keysym.sym] = false;

		break;
	default:
		Check(false); // what
	}
	
}

void InputManager::PushAction(SDL_Keysym Keysym)
{
	KeyMapping* Keymap = nullptr;
	if (Keymap = Utility::FindPred(KeyMappings, [=](KeyMapping* RHS) { return RHS->GetSymbol() == Keysym.sym; }))
	{
		for (const std::string& ActionName : Keymap->GetActionMappings())
		{
			if (ActionGroup* Group = Utility::FindPred(ActionGroups, [=](ActionGroup* RHS) { return RHS->GetActionName() == ActionName; }))
			{
				Logging::LogVerbose("InputManager::PushAction()", "Action \"" + ActionName + "\" invoked.");
				ActionInfo Info = ActionInfo();
				Group->Execute(Info);
			}
		}
	}
	else
	{
		//Logging::LogVerbose("InputManager::PushAction()", "Tried to push actions for key but there was no mapping.");
	}


}

bool InputManager::IsKeyHeld(SDL_Keycode Symbol)
{
	return InputMap[Symbol];
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

