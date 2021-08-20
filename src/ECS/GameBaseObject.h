#pragma once
#include <cstdint>
#include <stack>
#include <vector>

class GameBaseObject
{
public:
	GameBaseObject();

	const size_t& GetUID();

	bool operator==(const GameBaseObject& RHS);

	virtual void OnSpawn() {};
	virtual void OnDestroy() {};
	virtual void OnEnable() {};
	virtual void OnDisable() {};
	virtual void OnPreTick() {};
	virtual void OnTick() {};
	virtual void OnPostTick() {};

	void Enable();
	void Disable();
	void Destroy();

	// these are only for the GameManager to use
	static void SpawnPendingObjects();
	static void DestroyPendingObjects();
	static void TickAllObjects();

private:
	size_t UID;

	static size_t UIDCounter;

	static std::stack<GameBaseObject*> BaseObjectsPendingSpawn;
	static std::stack<GameBaseObject*> BaseObjectsPendingDestroy;
	static std::vector<GameBaseObject*> AllBaseObjects;

	bool bPendingDestroy = false;
	bool bEnabled = true;
};