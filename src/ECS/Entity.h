#pragma once
#include <vector>
#include "Component.h"

class Entity
{
public:
	template<class T>
	void AddComponent() { Components.push_back(new T()); }
	void RemoveComponent(Component* Comp);

private:
	std::vector<Component*> Components;
};