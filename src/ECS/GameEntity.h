#pragma once
#include "GameBaseObject.h"

class GameEntity : public GameBaseObject
{
public:
	virtual void OnTick(float DeltaTime) override;
};