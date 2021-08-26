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
	static void HandleMouseMotionEvent(SDL_MouseMotionEvent& MouseMotionEvent);
	static void HandleMouseWheelEvent(SDL_MouseWheelEvent& MouseWheelEvent);
	static void HandleMouseButtonEvent(SDL_MouseButtonEvent& MouseEvent);

	static void AddKeyMapping(std::string Action, SDL_Keycode Symbol);
	static void AddMouseButtonMapping(std::string Action, Uint8 Button);

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
	static bool IsMouseButtonHeld(Uint8 Button);
private:

	class ActionGroup;
	class KeyMapping;
	class MouseButtonMapping;

	static void PushAction(SDL_KeyboardEvent KBEvent);
	static void PushActionMouseButton(SDL_MouseButtonEvent MSBEvent);

	static std::unordered_map<SDL_Keycode, bool> KeyInputMap;
	static std::vector<InputManager::KeyMapping*> KeyMappings;

	static std::unordered_map<Uint8, bool> MouseButtonInputMap;
	static std::vector<InputManager::MouseButtonMapping*> MouseButtonMappings;

	static std::vector<InputManager::ActionGroup*> ActionGroups;
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

	class MouseButtonMapping
	{
	public:
		MouseButtonMapping(Uint8 Button);
		Uint8 GetButton();
		void AddMapping(std::string ActionName);
		const std::vector<std::string>& GetActionMappings();
	private:
		Uint8 Button;
		std::vector<std::string> ActionMappings;
	};
};

