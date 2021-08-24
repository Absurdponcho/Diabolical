#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include "../ECS/GameBaseObject.h"

enum class InputType
{
	IT_Pressed,
	IT_Released,
	IT_Held,
};


class ActionInfo
{
public:

};

class InputDelegate
{
public:
	size_t UID = 0;
	std::function<void(ActionInfo&)> Method;
	std::string Action;
	bool Execute(ActionInfo& Info)
	{
		if (GameBaseObject::GetFromUID(UID))
		{
			Method(Info);
			return true;
		}
		return false;
	}
};

class InputManager
{
public:
	static void HandleKeyboardEvent(SDL_KeyboardEvent& KeyboardEvent);
	static void AddKeyMapping(std::string Action, SDL_Keycode Symbol);
	static void BindFunction(std::string Action, std::function<void(ActionInfo&)> Function);

	static void BindInputDelegate(std::string Action, GameBaseObject* Object, std::function<void(ActionInfo&)> Meth);

	template <class UserClass>
	static void BindMethod(std::string Action, UserClass* Object, void (UserClass::* MethodPtr)(ActionInfo&))
	{
		static_cast<GameBaseObject*>(Object);
		std::function<void(ActionInfo&)> Fn = [=](ActionInfo& Info) 
		{
			(Object->*MethodPtr)(Info);
		};
		BindInputDelegate(Action, Object, Fn);
	}

	static bool IsKeyHeld(SDL_Keycode Symbol);
private:
	class ActionGroup;
	class KeyMapping;

	static void PushAction(SDL_Keysym Keycode);
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

