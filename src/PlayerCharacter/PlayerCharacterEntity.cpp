#include "PlayerCharacterEntity.h"
#include "../Physics/RigidbodyComponent.h"
#include "../Rendering/Particles/Particle.h"
#include "../Rendering/SpriteRendererComponent.h"

void PlayerCharacterEntity::OnSpawn()
{
	GameEntity::OnSpawn();

	InputManager::BindMethod("Jump", this, &PlayerCharacterEntity::Jump);
	InputManager::BindMethod("Right", this, &PlayerCharacterEntity::Right);
	InputManager::BindMethod("Left", this, &PlayerCharacterEntity::Left);
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
	Particle.Position = GetTransform().GetPosition();
	Particle.Size = 0.1f;
	constexpr float Pi = glm::pi<float>();
	for (int i = 0; i < 500; i++)
	{
		float Rand = (float)(rand() % 100);
		Particle.Color = glm::vec4(Rand / 50, Rand / 120, 0, 1);
		Particle.Speed = 5.5f + glm::mod<float>((float)rand(), 30.f) / 10;
		Particle.Rotation = Pi + (glm::mod<float>((float)rand(), Pi / 2) - Pi / 4);
		Particle.Lifetime = 30;
		Particle.RotationOverTime = ((glm::mod<float>((float)rand(), Pi / 2) - Pi / 4)) * 3;
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