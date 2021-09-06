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
#include <Physics/PhysicsWorld.h>
#include <box2d/b2_api.h>

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

	RigidbodyComponent* Rigidbody = GetComponent<RigidbodyComponent>();

	if (glm::length(Movement) > 0)
	{
		Rigidbody->SetVelocity(Utility::ConvertTob2Vec2(glm::normalize(Movement) * DeltaTime * MovementSpeed));
	}
	else
	{
		Rigidbody->SetVelocity(b2Vec2(0, 0));
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


			b2AABB AABB;
			AABB.lowerBound = b2Vec2(GetTransform().GetPosition().x - .5f, GetTransform().GetPosition().y - .8f);
			AABB.upperBound = b2Vec2(GetTransform().GetPosition().x + .5f, GetTransform().GetPosition().y + .8f);

			std::vector<GameEntity*> HitEntities;
			PhysicsWorld::Get().QueryAABB(AABB, HitEntities);
			for (GameEntity* HitEntity : HitEntities)
			{
				if (CharacterEntity* CharEntity = dynamic_cast<CharacterEntity*>(HitEntity))
				{
					CharEntity->Damage(40, this);
				}
			}
		}
	}

	EntityTransform& Transform = GetTransform();
	glm::vec3 NewPosition = Transform.GetPosition();

	if (NewPosition.x < Bounds.x) NewPosition.x = Bounds.x;
	if (NewPosition.y < Bounds.y) NewPosition.y = Bounds.y;
	if (NewPosition.x > Bounds.z) NewPosition.x = Bounds.z;
	if (NewPosition.y > Bounds.w) NewPosition.y = Bounds.w;

	Transform.SetPosition(NewPosition);

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