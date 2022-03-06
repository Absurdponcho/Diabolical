#pragma once
#include "Flecs/flecs.h"
#include "Check.h"
#include "Types/DString.h"

class DUtilityECS
{
public:
	static void InitializeECSSystems();
	static flecs::world& GetECSWorld();

private:
	static flecs::world ECSWorld;

};

struct DEntityData
{
	DString Name;
};

class DEntity
{
public:
	static DEntity CreatePrefab(DString Name = "")
	{
		DEntity NewPrefab;
		NewPrefab.FlecsEntity = DUtilityECS::GetECSWorld().prefab(*Name);
		DEntityData Data;
		Data.Name = Name;
		NewPrefab.Set(Data);
		return NewPrefab;
	}

	static DEntity CreateEntity(DString Name = "")
	{
		DEntity NewEntity;
		NewEntity.FlecsEntity = DUtilityECS::GetECSWorld().entity(*Name);
		DEntityData Data;
		Data.Name = Name;
		NewEntity.Set(Data);
		return NewEntity;
	}

	DEntity() {};

	DEntity(const DEntity& ExistingEntity)
	{
		*this = ExistingEntity;
		Check (FlecsEntity == ExistingEntity.FlecsEntity);
	}

	DEntity(flecs::entity ExistingEntity)
	{
		FlecsEntity = ExistingEntity;
	}

	void SetParent(DEntity NewParent)
	{
		Check(NewParent.IsAlive());
		Check(IsAlive());
		if (!IsAlive() || NewParent.IsAlive()) return;
		FlecsEntity.child_of(NewParent.FlecsEntity);
	}
	
	void Inherit(DEntity Parent)
	{
		Check(Parent.IsAlive());
		Check(IsAlive());
		if (!IsAlive() || Parent.IsAlive()) return;
		FlecsEntity.is_a(Parent.GetFlecsEntity());
	}

	bool IsAlive() const
	{
		return FlecsEntity.is_alive();
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
		Check (IsAlive());
		if (!IsAlive()) return nullptr;
		if (!FlecsEntity.has<T>()) return nullptr;
		return FlecsEntity.get_mut<T>();
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

protected:

	flecs::entity& GetFlecsEntity()
	{
		return FlecsEntity;
	}
	flecs::entity FlecsEntity;
};
