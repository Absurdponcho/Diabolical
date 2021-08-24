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
	virtual void OnPostPhysics(float DeltaTime) {};
	virtual void OnPreTick(float DeltaTime) {};
	virtual void OnTick(float DeltaTime) {};
	virtual void OnPostTick(float DeltaTime) {};
	virtual void OnPostRender(float DeltaTime) {};

	void Enable();
	void Disable();
	virtual void Destroy();
	bool IsPendingDestroy();
	bool IsEnabled();
	
	static GameBaseObject* GetFromUID(size_t TargetUID);

	// Never touch these!!
	static void SpawnPendingObjects();
	static void DestroyPendingObjects();
	static void TickAllObjects(float DeltaTime);
	static void PostRenderAllObjects(float DeltaTime);
	static bool bCreationLock;

private:
	size_t UID;

	static size_t UIDCounter;

	static std::stack<GameBaseObject*> BaseObjectsPendingSpawn;
	static std::stack<GameBaseObject*> BaseObjectsPendingDestroy;
	static std::vector<GameBaseObject*> AllBaseObjects;

	bool bPendingDestroy = false;
	bool bEnabled = true;
};
