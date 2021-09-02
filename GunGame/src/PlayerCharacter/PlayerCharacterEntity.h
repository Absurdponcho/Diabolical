#pragma once
#include <ECS/GameEntity.h>
#include <ECS/GameWeakObjectPointer.h>
#include <Input/InputManager.h>

class PlayerCharacterEntity : public GameEntity
{
public:
	PlayerCharacterEntity() {}
	virtual void OnSpawn() override;
	virtual void OnTick(float DeltaTime) override;

	void Jump(ActionInfo ActionInfo);
	void Right(ActionInfo ActionInfo);
	void Left(ActionInfo ActionInfo);
	void Shoot(ActionInfo ActionInfo);

	float HorizonalMovementForce = 2000;
	float JumpVelocity = 8;

	GameEntity* PlayerArm = nullptr;
	GameEntity* HeldItem = nullptr;

private:

	float HorizontalMovement = 0;
};