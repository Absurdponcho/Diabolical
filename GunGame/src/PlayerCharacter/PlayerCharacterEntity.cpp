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
#include "BulletComponent.h"
GameAssetSoftPointer<TextureAsset> ArcaneBulletTexturePointer("GameAssetFiles/ArcaneBullet.png");
GameAssetSoftPointer<AudioAsset> PhaserSoundPointer("GameAssetFiles/phaser1.wav");
void PlayerCharacterEntity::OnSpawn()
{
	GameEntity::OnSpawn();

	InputManager::BindMethod("Jump", this, &PlayerCharacterEntity::Jump);
	InputManager::BindMethod("Right", this, &PlayerCharacterEntity::Right);
	InputManager::BindMethod("Left", this, &PlayerCharacterEntity::Left);
	InputManager::BindMethod("Shoot", this, &PlayerCharacterEntity::Shoot);
}

void PlayerCharacterEntity::OnTick(float DeltaTime)
{
	Check(PlayerArm);
	Check(HeldItem);

	if (HorizontalMovement != 0)
	{
		b2Vec2 Force(glm::clamp(HorizontalMovement, -1.f, 1.f) * DeltaTime * HorizonalMovementForce, 0.f);
		GetComponent<RigidbodyComponent>()->AddForceAtCenter(Force);
	}

	glm::ivec2 ScreenPos;
	SDL_GetMouseState((int*)&ScreenPos, (int*)&ScreenPos + 1);
	glm::vec3 WorldPosition = Utility::ScreenToWorld(ScreenPos, CameraComponent::GetActiveCamera());

	if (WorldPosition.x > GetTransform().GetPosition().x)
	{
		GetComponent<SpriteRendererComponent>()->bXMirrored = false;
		PlayerArm->GetComponent<SpriteRendererComponent>()->bXMirrored = false;
		PlayerArm->GetTransform().SetPosition(glm::vec3(-0.05f, -0.05f, 0.01f));
		HeldItem->GetComponent<SpriteRendererComponent>()->bYMirrored = false;

	}
	else
	{
		GetComponent<SpriteRendererComponent>()->bXMirrored = true;
		PlayerArm->GetComponent<SpriteRendererComponent>()->bXMirrored = true;
		PlayerArm->GetTransform().SetPosition(glm::vec3(0.05f, -0.05f, 0.01f));
		HeldItem->GetComponent<SpriteRendererComponent>()->bYMirrored = true;

	}

	PlayerArm->GetTransform().SetEulerRotation(glm::vec3(0, 0, Utility::AngleBetween(GetTransform().GetPosition(), glm::vec2(WorldPosition)) * 57.2958f + 90));

}

void PlayerCharacterEntity::Jump(ActionInfo ActionInfo)
{
	if (ActionInfo.InputType != EInputType::IT_Pressed) return;
	b2Vec2 Force(0, JumpVelocity);
	RigidbodyComponent* Rigidbody = GetComponent<RigidbodyComponent>();
	Rigidbody->SetVelocity(b2Vec2(Rigidbody->GetVelocity().x, JumpVelocity));

	Particle Particle;
	//Particle.Color = glm::vec4((float)(rand() % 100) / 100, (float)(rand() % 100) / 100, (float)(rand() % 100) / 100, 1);
	Particle.Position = GetTransform().GetPosition() - glm::vec3(0, .6f, 0);
	Particle.Size = 1.f / 16.f;
	constexpr float Pi = glm::pi<float>();
	for (int i = 0; i < 100; i++)
	{
		float Rand = (float)(rand() % 100);
		Particle.Color = glm::vec4(Rand / 50, Rand / 120, 0, 1);
		Particle.Speed = 2.5f + glm::mod<float>((float)rand(), 30.f) / 10;
		Particle.Rotation = -Pi / 2 + (glm::mod<float>((float)rand(), Pi / 2) - Pi / 4);
		Particle.Lifetime = Rand / 100;
		Particle.RotationOverTime = ((glm::mod<float>((float)rand(), Pi / 2) - Pi / 4)) * 3;
		Particle.Acceleration = -5;
		ParticleManager::RegisterParticle(Particle);
	}
}

void PlayerCharacterEntity::Right(ActionInfo ActionInfo)
{
	if (ActionInfo.InputType == EInputType::IT_Pressed)
	{
		HorizontalMovement += 1;
	}
	else if (ActionInfo.InputType == EInputType::IT_Released)
	{
		HorizontalMovement -= 1;
	}
}

void PlayerCharacterEntity::Left(ActionInfo ActionInfo)
{
	if (ActionInfo.InputType == EInputType::IT_Pressed)
	{
		HorizontalMovement -= 1;
	}
	else if (ActionInfo.InputType == EInputType::IT_Released)
	{
		HorizontalMovement += 1;
	}
}

void PlayerCharacterEntity::Shoot(ActionInfo ActionInfo)
{
	if (ActionInfo.MouseButtonEvent->state == SDL_PRESSED) {
		for (int i = 0; i < 20; i++)
		{
			GameEntity* ArcaneBullet = CreateEntity<GameEntity>();
			{
				RigidbodyComponent* Rigidbody = CreateComponent<RigidbodyComponent>(ArcaneBullet);
				Rigidbody->SetDynamic(true);
				Rigidbody->SetRotates(false);

				SpriteRendererComponent* Sprite = CreateComponent<SpriteRendererComponent>(ArcaneBullet);
				Sprite->SetTexture(ArcaneBulletTexturePointer);
				Sprite->SpriteSheetSize = glm::ivec2(4, 1);
				Sprite->SpriteSheetProgressionSpeed = 8;

				Check(HeldItem);
				auto pos = HeldItem->GetTransform().GetWorldPosition();
				auto euler = HeldItem->GetTransform().GetWorldEulerRotation() + glm::vec3(0, 0, 90);

				ArcaneBullet->GetTransform().SetPosition(pos);
				ArcaneBullet->GetTransform().SetEulerRotation(euler);

				glm::ivec2 ScreenPos;
				SDL_GetMouseState((int*)&ScreenPos, (int*)&ScreenPos + 1);
				glm::vec3 MouseWorldPosition = Utility::ScreenToWorld(ScreenPos, CameraComponent::GetActiveCamera());
				glm::vec2 ForceDirection = glm::normalize(MouseWorldPosition - pos);

				ColliderComponent* Collider = CreateComponent<ColliderComponent>(ArcaneBullet);
				Collider->SetSize(b2Vec2(.1f, .1f));
				Collider->SetDensity(1);

				CreateComponent<BulletComponent>(ArcaneBullet);

				b2Vec2 Force = Utility::ConvertTob2Vec2(ForceDirection * 30.f);
				Rigidbody->SetVelocity(Force);
				GameAudio::PlaySound(PhaserSoundPointer);
			}
		}
	}
	
}
