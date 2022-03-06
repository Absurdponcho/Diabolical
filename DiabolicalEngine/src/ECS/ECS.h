#pragma once
#include "Flecs/flecs.h"

typedef flecs::entity DEntity;

class DUtilityECS 
{
public:
	static void InitializeECSSystems();
	static flecs::world& GetECSWorld();

private:
	static flecs::world ECSWorld;

};