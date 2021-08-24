#pragma once
#include <functional>
#include "../ECS/GameBaseObject.h"
#include "../Logging/Logging.h"

template <typename ReturnValue, typename... Parameters>
class GameDelegate
{
public:
	template<class UserClass>
	void Bind(UserClass* Object, ReturnValue(UserClass::* Method)(Parameters...))
	{
		std::function<ReturnValue(Parameters...)> NewFunc = [=](Parameters... param) 
		{ 
			return (Object->*Method)(param...); 
		};
		UID = Object->GetUID();
		Function = NewFunc;
	}

	ReturnValue Execute(Parameters... Args)
	{
		if (GameBaseObject::GetFromUID(UID))
		{
			return Function(Args...);
		}
		else
		{
			UID = 0;
		}
		return ReturnValue();
	}

	void Unbind()
	{
		UID = 0;
	}

	bool IsBound()
	{
		return UID > 0;
	}

private:
	size_t UID = 0;
	std::function<ReturnValue(Parameters...)> Function;

};



template <typename... Parameters>
class GameMulticastDelegate
{
private:
	template <typename... Parameters>
	struct MulticastDelegateBinding
	{
	public:
		size_t UID = 0;
		std::function<void(Parameters...)> Function;
	};
public:
	template<class UserClass>
	void Bind(UserClass* Object, void(UserClass::* Method)(Parameters...))
	{
		GameBaseObject* GBO = static_cast<GameBaseObject*>(Object);
		
		GameMulticastDelegate::MulticastDelegateBinding<Parameters...> Binding;
		Binding.UID = GBO->GetUID();
		Binding.Function = [=](Parameters... param)
		{
			(Object->*Method)(param...);
		};
		Bindings.push_back(Binding);
	}

	void Execute(Parameters... Args)
	{
		for (GameMulticastDelegate::MulticastDelegateBinding<Parameters...>& Binding : Bindings)
		{
			if (GameBaseObject::GetFromUID(Binding.UID))
			{
				Binding.Function(Args...);
			}
			else
			{
				Unbind(Binding.UID);
			}
		}
	}

	void Unbind(GameBaseObject* Object)
	{
		for (int Index = 0; Index < Bindings.size(); Index++)
		{
			if (Bindings[Index].UID == Object->UID)
			{
				Bindings.erase(Bindings.begin() + Index);
				return;
			}
		}
	}
	void Unbind(size_t UID)
	{
		for (int Index = 0; Index < Bindings.size(); Index++)
		{
			if (Bindings[Index].UID == UID)
			{
				Bindings.erase(Bindings.begin() + Index);
				return;
			}
		}
	}

	void UnbindAll()
	{
		Bindings.clear();
	}

	bool IsBoundTo(GameBaseObject* Object)
	{
		for (int Index = 0; Index < Bindings.size(); Index++)
		{
			if (Bindings[Index].UID == Object->GetUID())
			{
				return true;
			}
		}
		return false;
	}

	bool IsBound()
	{
		return Bindings.size() > 0;
	}

private:
	std::vector<GameMulticastDelegate::MulticastDelegateBinding<Parameters...>> Bindings;

};