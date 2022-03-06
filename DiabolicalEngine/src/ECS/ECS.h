#pragma once
#include "Flecs/flecs.h"

class DUtilityECS 
{
public:
	static void InitializeECSSystems();
	static flecs::entity SceneRoot;

	static flecs::world& GetECSWorld();

private:
	static flecs::world ECSWorld;

};