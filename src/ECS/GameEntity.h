#pragma once
#include <vector>

class GameComponent;

class GameEntity
{
public:
	template<class T>
	void AddComponent() { Components.push_back(new T()); }
	void RemoveComponent(GameComponent* Comp);

private:
	std::vector<GameComponent*> Components;
};