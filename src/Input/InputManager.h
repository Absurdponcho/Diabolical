#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include "../ECS/GameBaseObject.h"
#include "../Delegate/GameDelegate.h"
#include "../Utility/Utility.h"

enum class EInputType
{
	IT_Pressed,
	IT_Released,
	IT_Held
};


struct ActionInfo
{
public:
	EInputType InputType;
};

typedef GameMulticastDelegate<ActionInfo> InputDelegate;

class InputManager
{
public:
	static void HandleKeyboardEvent(SDL_KeyboardEvent& KeyboardEvent);
	static void AddKeyMapping(std::string Action, SDL_Keycode Symbol);

	template<class UserClass>
	static void BindMethod(std::string Action, UserClass* Object, void (UserClass::* MethodPtr)(ActionInfo))
	{
		InputManager::ActionGroup* Group = nullptr;
		if (!(Group = Utility::FindPred(ActionGroups, [=](InputManager::ActionGroup* RHS) {return RHS->GetActionName() == Action; })))
		{
			Group = new InputManager::ActionGroup(Action);
			ActionGroups.push_back(Group);
		}

		InputDelegate Delegate = InputDelegate();
		Delegate.Bind(Object, MethodPtr);
		InputDelegates.push_back(Delegate);

		Group->BindInputDelegate(Delegate);
	}

	static bool IsKeyHeld(SDL_Keycode Symbol);
private:

	class ActionGroup;
	class KeyMapping;

	static void PushAction(SDL_KeyboardEvent KBEvent);
	static std::unordered_map<SDL_Keycode, bool> InputMap;
	static std::vector<InputManager::ActionGroup*> ActionGroups;
	static std::vector<InputManager::KeyMapping*> KeyMappings;
	static std::vector<InputDelegate> InputDelegates;

	class ActionGroup
	{
	public:
		ActionGroup(std::string Action);
		void BindFunction(std::function<void(ActionInfo&)> Function);
		void BindInputDelegate(InputDelegate Delegate);
		const std::string& GetActionName();
		void Execute(ActionInfo& Info);
	private:
		std::string ActionName;
		std::vector<std::function<void(ActionInfo&)>> FunctionBindings;
		std::vector<InputDelegate> InputDelegateBindings;
	};

	class KeyMapping
	{
	public:
		KeyMapping(SDL_Keycode Symbol);
		SDL_Keycode GetSymbol();
		void AddMapping(std::string ActionName);
		const std::vector<std::string>& GetActionMappings();
	private:
		SDL_Keycode KeySymbol;
		std::vector<std::string> ActionMappings;
	};
};

