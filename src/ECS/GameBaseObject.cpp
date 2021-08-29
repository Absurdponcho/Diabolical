#include "GameBaseObject.h"
#include "../Logging/Logging.h"
#include "../GunGame.h"
#include "../Utility/Utility.h"

size_t GameBaseObject::UIDCounter = 1;

std::vector<GameBaseObject*> GameBaseObject::BaseObjectsPendingSpawn;
std::stack<GameBaseObject*> GameBaseObject::BaseObjectsPendingDestroy;
std::vector<GameBaseObject*> GameBaseObject::AllBaseObjects;
std::unordered_map<size_t, GameBaseObject*> GameBaseObject::BaseObjectsMap;
bool GameBaseObject::bCreationLock = true;

GameBaseObject::GameBaseObject()
	: UID(UIDCounter++)
{
	Check(!bCreationLock);
	BaseObjectsPendingSpawn.push_back(this);
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
	for (GameBaseObject* BaseObject : BaseObjectsPendingSpawn)
	{
		Logging::LogVerbose("GameBaseObject::SpawnPendingObjects()", "Spawned object with UID " + std::to_string(BaseObject->GetUID()));
		AllBaseObjects.push_back(BaseObject);
		BaseObjectsMap[BaseObject->UID] = BaseObject;
		BaseObject->OnSpawn();
	}

	BaseObjectsPendingSpawn.clear();
}

void GameBaseObject::DestroyPendingObjects()
{
	while (!BaseObjectsPendingDestroy.empty())
	{
		GameBaseObject* BaseObject = BaseObjectsPendingDestroy.top();
		size_t UID = BaseObject->UID;

		if (!BaseObject) continue;
		BaseObjectsPendingDestroy.pop();
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

		BaseObjectsMap.erase(UID);

		delete(BaseObject);
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

void GameBaseObject::TickPhysics(float FixedDeltaTime)
{
	for (GameBaseObject* BaseObject : AllBaseObjects)
	{
		if (!BaseObject->bEnabled) continue;
		if (BaseObject->bPendingDestroy) continue;
		BaseObject->OnPostPhysics(FixedDeltaTime);
	}
}

void GameBaseObject::PostRenderAllObjects(float DeltaTime)
{
	for (GameBaseObject* BaseObject : AllBaseObjects)
	{
		if (!BaseObject->bEnabled) continue;
		if (BaseObject->bPendingDestroy) continue;
		BaseObject->OnPostRender(DeltaTime);
	}
}

bool GameBaseObject::IsPendingDestroy()
{
	return bPendingDestroy;
}

bool GameBaseObject::IsEnabled()
{
	return bEnabled;
}

GameBaseObject* GameBaseObject::GetFromUID(size_t TargetUID)
{
	return BaseObjectsMap[TargetUID];
}