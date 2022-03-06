#include "ECS.h"
#include "Graphics/Rendering/MeshRendererComponent.h"
#include "Graphics/Rendering/Camera.h"
#include "Maths/Maths.h"
#include "Graphics/Rendering/Scene.h"

flecs::world DUtilityECS::ECSWorld;

void DUtilityECS::InitializeECSSystems()
{
	DScene::InitECSSystems();
	DMeshRendererComponent::InitECSSystems();
	DCameraComponent::InitECSSystems();
}

flecs::world& DUtilityECS::GetECSWorld()
{
	return ECSWorld;
}

DEntity DEntity::CreatePrefab(DString Name)
{
	DEntity NewPrefab;
	NewPrefab.FlecsEntity = DUtilityECS::GetECSWorld().prefab(*Name);
	DEntityData Data;
	Data.Name = Name;
	NewPrefab.Set(Data);
	return NewPrefab;
}

DEntity DEntity::CreateEntity(DString Name)
{
	DEntity NewEntity;
	NewEntity.FlecsEntity = DUtilityECS::GetECSWorld().entity(*Name);
	DEntityData Data;
	Data.Name = Name;
	NewEntity.Set(Data);
	return NewEntity;
}


void DEntity::SetParent(DEntity& NewParent)
{
	NewParent.AddChild(*this);
}

void DEntity::AddChild(DEntity& NewChild)
{
	Check(NewChild.IsAlive());
	Check(IsAlive());
	if (!IsAlive() || !NewChild.IsAlive()) return;
	Check(!HasChild(NewChild));
	if (HasChild(NewChild)) return;

	DEntityData& ChildEntityData = NewChild.GetEntityData();

	if (ChildEntityData.Parent.IsAlive())
	{
		ChildEntityData.Parent.RemoveChild(NewChild);
	}

	ChildEntityData.Parent = *this;
	GetEntityData().Children.PushBack(NewChild);

	NewChild.FlecsEntity.child_of(FlecsEntity);
}

void DEntity::RemoveChild(DEntity& Child)
{
	Check(Child.IsAlive());
	Check(IsAlive());
	if (!IsAlive() || !Child.IsAlive()) return;
	Check(HasChild(Child));
	if (!HasChild(Child)) return;

	Child.FlecsEntity.remove(flecs::ChildOf, FlecsEntity);
	Child.GetEntityData().Parent = DEntity();
	GetEntityData().Children.Remove(Child);

}

bool DEntity::HasChild(const DEntity& Child)
{
	Check(Child.IsAlive());
	Check(IsAlive());
	if (!IsAlive() || !Child.IsAlive()) return false;

	return GetEntityData().Children.Contains(Child);
}

DEntity& DEntity::GetParent()
{
	return GetEntityData().Parent;
}

void DEntity::DetachFromParent()
{
	Check(IsAlive());
	Check(GetParent().IsAlive());
	if (!IsAlive() || !GetParent().IsAlive()) return;

	GetParent().RemoveChild(*this);
}

void DEntity::Inherit(DEntity Parent)
{
	LOG_ERR("DEntity::Inherit is temporarily disabled. Use DEntity::Clone().");
	Check(false);
	Check(Parent.IsAlive());
	Check(IsAlive());
	if (!IsAlive() || Parent.IsAlive()) return;
	FlecsEntity.is_a(Parent.GetFlecsEntity());

}

DString& DEntity::GetName()
{
	return GetEntityData().Name;
}

DEntity DEntity::Clone(DString NewName)
{
	Check(IsAlive());
	
	DString Name = NewName;
	if (Name.Length() == 0)
	{
		Name = DString::Format("%s_Clone", *GetName());
	}

	// create empty entity
	DEntity NewEntity = DEntity::CreateEntity(Name);

	// clone components into new entity components
	ecs_clone(FlecsEntity.m_world, NewEntity.FlecsEntity, FlecsEntity, true);

	// set name 
	NewEntity.GetEntityData().Name = Name;

	// remove possible prefab meme
	NewEntity.FlecsEntity.remove(flecs::Prefab);
	return NewEntity;
}