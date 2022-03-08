#pragma once
#include "Flecs/flecs.h"
#include "Check.h"
#include "Types/DString.h"
#include "Types/DVector.h"
#include "Types/Meta.h"

class DEntity;
struct DEntityData;

class DUtilityECS
{
public:
	static void InitializeECSSystems();
	static flecs::world& GetECSWorld();

private:
	static flecs::world ECSWorld;

};


class DEntity
{
public:
	inline static size_t GetSize(){ return sizeof(DEntity); }

	// Creates a new prefab entity. This is the exact same as an entity, except it is excluded from systems and queries.
	static DEntity CreatePrefab(const DString& OriginalName = "");

	// Creates a new entity. Add data by adding components, add functionality by adding systems.
	static DEntity CreateEntity(const DString& Name = "");

	static DString GenerateUniqueName(const DString& Name);

	DEntity() {};

	// Makes this DEntity the same entity as ExistingEntity (does not clone data)
	DEntity(const DEntity& ExistingEntity)
	{
		*this = ExistingEntity;
		Check (FlecsEntity == ExistingEntity.FlecsEntity);
	}

	DEntity(flecs::entity ExistingEntity)
	{
		FlecsEntity = ExistingEntity;
	}

	DString& GetName();

	DEntity Clone(DString NewName = "");

	// Set the parent entity of this entity.
	// Each entity can only have 1 parent.
	void SetParent(DEntity& NewParent);

	// Add a child entity to this entity.
	// The child entity will have this entity set as its parent.
	void AddChild(DEntity& NewChild);

	// Remove a child entity from this entity.
	// The child will no longer have this entity as its parent.
	void RemoveChild(DEntity& Child);

	// Remove the parent from this entity.
	// The parent will have this entity removed from its child list.
	void DetachFromParent();

	// Returns true if this entity is the parent of the specified child.
	bool HasChild(const DEntity& Child);
	
	DEntity& GetParent();

	bool HasParent();

	// Inherits an existing entity. 
	// Inheriting the entity means this entity will have all of the same components.
	void Inherit(DEntity Parent);

	// Check if this entity is valid or not.
	bool IsAlive() const
	{
		return FlecsEntity.is_alive() ;
	}

	DEntityData& GetEntityData() const
	{
		return *FlecsEntity.get_mut<DEntityData>();
	}

	// Add a new component. If the entity already has the component, it will set the values.
	template<typename T>
	bool Set(T Component)
	{
		Check (IsAlive());
		if (!IsAlive()) return false;
		int Size = sizeof(T);
		if (std::is_empty<T>())
		{
			FlecsEntity.add<T>();
		}
		else
		{
			FlecsEntity.set<T>(Component);
		}
		return true;
	}

	// Gets a mutable pointer to the component. This will raise an exception in some situations.
	template<typename T>
	T* GetComponentMutable()
	{
		Check(IsAlive());
		if (!IsAlive()) return nullptr;
		Check (FlecsEntity.has<T>());
		if (!FlecsEntity.has<T>()) return nullptr;

		T* Comp = FlecsEntity.get_mut<T>();
		return Comp;

	}

	// Gets a const pointer to the component. This shouldn't fail.
	template<typename T>
	const T* GetComponent() const
	{

		Check (IsAlive());
		if (!IsAlive()) return nullptr;
		if (!FlecsEntity.has<T>()) return nullptr;
		return FlecsEntity.get<T>();
	}

	inline friend bool operator== (const DEntity& LHS, const DEntity& RHS)
	{
		return LHS.FlecsEntity == RHS.FlecsEntity;
	}


	flecs::entity GetFlecsEntity() const
	{
		return FlecsEntity;
	}

protected:
	flecs::entity FlecsEntity;

};

struct DEntityData
{
	DString Name;
	DEntity Parent;
	DVector<DEntity> Children;
};


// Any extra data that DEntity stores should be kept inside of DEntityData as a component.
// DEntity should be able to be copied and still point to the same entity, and have the same data.
static_assert(sizeof(DEntity) == sizeof(flecs::entity));