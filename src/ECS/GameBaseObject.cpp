#include "GameBaseObject.h"
#include "../Logging/Logging.h"
#include "../GunGame.h"

size_t GameBaseObject::UIDCounter = 0;

std::stack<GameBaseObject*> GameBaseObject::BaseObjectsPendingSpawn;
std::stack<GameBaseObject*> GameBaseObject::BaseObjectsPendingDestroy;
std::vector<GameBaseObject*> GameBaseObject::AllBaseObjects;
bool GameBaseObject::bCreationLock = true;

GameBaseObject::GameBaseObject()
	: UID(UIDCounter++)
{
	Check(!bCreationLock); // you HAVE to call GameBaseObject::CreateBaseObject() to ensure it is allocated correctly.
	BaseObjectsPendingSpawn.push(this);
}

const size_t& GameBaseObject::GetUID()
{
	return UID;
}

bool GameBaseObject::operator==(const GameBaseObject& RHS)
{
	return UID == RHS.UID;
}

void GameBaseObject::Destroy()
{
	Check(!bPendingDestroy);

	bPendingDestroy = true;
	BaseObjectsPendingDestroy.push(this);
}

void GameBaseObject::Enable()
{
	Check(!bPendingDestroy);
	if (bEnabled) return;
	bEnabled = true;
	OnEnable();
}
void GameBaseObject::Disable()
{
	Check(!bPendingDestroy);
	if (!bEnabled) return;
	bEnabled = false;
	OnDisable();
}

void GameBaseObject::SpawnPendingObjects()
{
	while (!BaseObjectsPendingSpawn.empty())
	{
		GameBaseObject* BaseObject = BaseObjectsPendingSpawn.top();
		BaseObject->OnSpawn();
		Logging::LogVerbose("GameBaseObject::SpawnPendingObjects()", "Spawned object with UID " + std::to_string(BaseObject->GetUID()));
		AllBaseObjects.push_back(BaseObject);
		BaseObjectsPendingSpawn.pop();
	}
}

void GameBaseObject::DestroyPendingObjects()
{
	while (!BaseObjectsPendingDestroy.empty())
	{
		GameBaseObject* BaseObject = BaseObjectsPendingDestroy.top();
		BaseObject->OnDestroy();
		Logging::LogVerbose("GameBaseObject::SpawnPendingObjects()", "Destroyed object with UID " + std::to_string(BaseObject->GetUID()));

		for (int Index = 0; Index < AllBaseObjects.size(); Index++)
		{
			if (*BaseObject == *AllBaseObjects[Index])
			{
				AllBaseObjects.erase(AllBaseObjects.begin() + Index);
				break;
			}
		}

		delete(BaseObject);
		BaseObjectsPendingDestroy.pop();
	}
}

void GameBaseObject::TickAllObjects(float DeltaTime)
{
	for (GameBaseObject* BaseObject : AllBaseObjects)
	{
		if (!BaseObject->bEnabled) continue;
		if (BaseObject->bPendingDestroy) continue;
		BaseObject->OnPreTick(DeltaTime);
	}	

	for (GameBaseObject* BaseObject : AllBaseObjects)
	{
		if (!BaseObject->bEnabled) continue;
		if (BaseObject->bPendingDestroy) continue;
		BaseObject->OnTick(DeltaTime);
	}	

	for (GameBaseObject* BaseObject : AllBaseObjects)
	{
		if (!BaseObject->bEnabled) continue;
		if (BaseObject->bPendingDestroy) continue;
		BaseObject->OnPostTick(DeltaTime);
	}
}