#pragma once
#include <ECS/GameEntity.h>
#include <ECS/GameWeakObjectPointer.h>
#include <Input/InputManager.h>

enum class EMovementState
{
	MS_Idle,
	MS_Running,
	MS_Attacking,
};

class PlayerCharacterEntity : public GameEntity
{
public:
	PlayerCharacterEntity() {}
	virtual void OnSpawn() override;
	virtual void OnTick(float DeltaTime) override;

	void Up(ActionInfo ActionInfo);
	void Right(ActionInfo ActionInfo);
	void Left(ActionInfo ActionInfo);
	void Down(ActionInfo ActionInfo);
	void Attack(ActionInfo ActionInfo);

	float MovementSpeed = 350;

private:
	float AttackMovementSpeed = 200;
	float DefaultMovementSpeed = 350;
	float AttackTimestamp = 0;
	float TimeBeforeWalkAnimationAfterAttack = 0.5f;
	void RecheckMovementState();
	void StartRunning();
	void StartIdle();
	void FinishAttack();

	bool bHoldingAttack = false;

	EMovementState MovementState;

	glm::vec2 Movement = { 0, 0 };
};