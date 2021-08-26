#pragma once
#include "../ECS/GameEntity.h"
#include "../ECS/GameWeakObjectPointer.h"
#include "../Input/InputManager.h"

class PlayerCharacterEntity : public GameEntity
{
public:
	PlayerCharacterEntity() {}
	virtual void OnSpawn() override;
	virtual void OnTick(float DeltaTime) override;

	void Jump(ActionInfo ActionInfo);
	void Right(ActionInfo ActionInfo);
	void Left(ActionInfo ActionInfo);

	float HorizonalMovementForce = 300;
	float JumpVelocity = 8;

private:

	float HorizontalMovement = 0;
};