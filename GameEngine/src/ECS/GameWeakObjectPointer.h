#pragma once
#include "../ECS/GameBaseObject.h"

template <class UserClass>
class GameWeakObjectPointer
{
public:
	GameWeakObjectPointer() : UID(0) {}

	GameWeakObjectPointer(UserClass* Object)
	{
		static_cast<GameBaseObject*>(Object);
		Check(Object);
		UID = Object->GetUID();
	}

	UserClass* Get()
	{
		if (UID == 0) return nullptr;
		return static_cast<UserClass*>(GameBaseObject::GetFromUID(UID));
	}

private:
	size_t UID;
};