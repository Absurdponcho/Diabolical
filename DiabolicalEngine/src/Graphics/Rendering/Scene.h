#pragma once
#include "ECS/ECS.h"
#include "Maths/Maths.h"

class DScene 
{
public:
	static void InitECSSystems();
	static DEntity SceneRoot;

private:
	static flecs::query<Transform3D, Transform3D> TransformQuery;

	static void TransformSystem(DEntity ParentEntity, Transform3D& Parent, Transform3D& Child);
};