#include "PlayerCharacterEntity.h"
#include <Physics/RigidbodyComponent.h>
#include <Physics/ColliderComponent.h>
#include <Rendering/Particles/Particle.h>
#include <Rendering/SpriteRendererComponent.h>
#include "glm/glm.hpp"
#include <WindowManager.h>
#include <Audio\AudioAsset.h>
#include <Audio/GameAudio.h>
#include <Utility/GameMath.h>
#include <GameManager.h>

void PlayerCharacterEntity::OnSpawn()
{
	GameEntity::OnSpawn();

	InputManager::BindMethod("Up", this, &PlayerCharacterEntity::Up);
	InputManager::BindMethod("Right", this, &PlayerCharacterEntity::Right);
	InputManager::BindMethod("Left", this, &PlayerCharacterEntity::Left);
	InputManager::BindMethod("Down", this, &PlayerCharacterEntity::Down);
	InputManager::BindMethod("AttackPrimary", this, &PlayerCharacterEntity::Attack);
}

void PlayerCharacterEntity::OnTick(float DeltaTime)
{
	if (Movement.x > 1) Movement.x = 1;
	if (Movement.x < -1) Movement.x = -1;	
	if (Movement.y > 1) Movement.y = 1;
	if (Movement.y < -1) Movement.y = -1;

	if (glm::length(Movement) > 0)
	{
		GetComponent<RigidbodyComponent>()->SetVelocity(Utility::ConvertTob2Vec2(glm::normalize(Movement) * DeltaTime * MovementSpeed));
	}
	else
	{
		GetComponent<RigidbodyComponent>()->SetVelocity(b2Vec2(0, 0));
	}

	if (bHoldingAttack)
	{
		if (MovementState != EMovementState::MS_Attacking)
		{
			MovementState = EMovementState::MS_Attacking;
			GetComponent<SpriteRendererComponent>()->SpriteLoopStart = 16;
			GetComponent<SpriteRendererComponent>()->SpriteLoopEnd = 21;
			AttackTimestamp = GameManager::GetTime();
			MovementSpeed = AttackMovementSpeed;
		}
	}

	FinishAttack();
}

void PlayerCharacterEntity::FinishAttack()
{
	if (GameManager::GetTime() > AttackTimestamp + TimeBeforeWalkAnimationAfterAttack && MovementState == EMovementState::MS_Attacking)
	{
		MovementSpeed = DefaultMovementSpeed;

		MovementState = EMovementState::MS_Idle;
		StartIdle();
		RecheckMovementState();

	}
}

void PlayerCharacterEntity::Up(ActionInfo ActionInfo)
{
	if (ActionInfo.InputType == EInputType::IT_Pressed)
	{
		Movement += glm::vec2(0, 1);
	}
	else if (ActionInfo.InputType == EInputType::IT_Released)
	{
		Movement += glm::vec2(0, -1);
	}

	RecheckMovementState();
}

void PlayerCharacterEntity::Right(ActionInfo ActionInfo)
{
	if (ActionInfo.InputType == EInputType::IT_Pressed)
	{
		Movement += glm::vec2(1, 0);
	}
	else if (ActionInfo.InputType == EInputType::IT_Released)
	{
		Movement += glm::vec2(-1, 0);
	}

	RecheckMovementState();
}

void PlayerCharacterEntity::Left(ActionInfo ActionInfo)
{
	if (ActionInfo.InputType == EInputType::IT_Pressed)
	{
		Movement += glm::vec2(-1, 0);
	}
	else if (ActionInfo.InputType == EInputType::IT_Released)
	{
		Movement += glm::vec2(1, 0);
	}

	RecheckMovementState();
}

void PlayerCharacterEntity::Down(ActionInfo ActionInfo)
{
	
	if (ActionInfo.InputType == EInputType::IT_Pressed)
	{
		Movement += glm::vec2(0, -1);
	}
	else if (ActionInfo.InputType == EInputType::IT_Released)
	{
		Movement += glm::vec2(0, 1);
	}

	RecheckMovementState();
}

void PlayerCharacterEntity::Attack(ActionInfo ActionInfo)
{
	if (ActionInfo.InputType == EInputType::IT_Pressed)
	{
		bHoldingAttack = true;

	}
	else if (ActionInfo.InputType == EInputType::IT_Released)
	{
		bHoldingAttack = false;
	}
}

void PlayerCharacterEntity::RecheckMovementState()
{
	switch (MovementState)
	{
	case EMovementState::MS_Idle:
		if (glm::length(Movement) > 0)
		{
			StartRunning();
		}
		break;
	case EMovementState::MS_Running:
		if (glm::length(Movement) < 0.0001f)
		{
			StartIdle();
		}
		break;
	}

	if (Movement.x > 0)
	{
		GetComponent<SpriteRendererComponent>()->bXMirrored = false;
	}
	else if (Movement.x < 0)
	{
		GetComponent<SpriteRendererComponent>()->bXMirrored = true;
	}
}

void PlayerCharacterEntity::StartIdle()
{
	if (MovementState == EMovementState::MS_Attacking) return;

	MovementState = EMovementState::MS_Idle;
	GetComponent<SpriteRendererComponent>()->SpriteLoopStart = 0;
	GetComponent<SpriteRendererComponent>()->SpriteLoopEnd = 3;
}

void PlayerCharacterEntity::StartRunning()
{
	if (MovementState == EMovementState::MS_Attacking) return;

	MovementState = EMovementState::MS_Running;
	GetComponent<SpriteRendererComponent>()->SpriteLoopStart = 8;
	GetComponent<SpriteRendererComponent>()->SpriteLoopEnd = 14;
}