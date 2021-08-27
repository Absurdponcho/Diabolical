#include <box2d/b2_api.h>
#include "PhysicsWorld.h"
#include  <box2d/b2_world.h>
#include "../Utility/Utility.h"
#include "../Logging/Logging.h"
#include "RigidbodyComponent.h"
#include "..\WindowManager.h"
#include "..\Rendering\Camera.h"

void RigidbodyComponent::OnSpawn()
{
	bSpawned = true;

	GameEntity& Parent = GetParentEntity();
	b2BodyDef BodyDef;
	BodyDef.type = b2BodyType::b2_dynamicBody;
	BodyDef.position.Set(Parent.GetTransform().Position.x, Parent.GetTransform().Position.y);
	BodyDef.angle = Parent.GetTransform().GetEulerRotation().z * 0.0174533f; // deg to rad

	Body = PhysicsWorld::Get().GetWorld().CreateBody(&BodyDef);

	Box.SetAsBox(Parent.GetTransform().Scale.x / 2, Parent.GetTransform().Scale.y / 2);

	b2FixtureDef FixtureDef;
	FixtureDef.shape = &Box;
	FixtureDef.density = 1.0f;
	FixtureDef.friction = 0.3f;

	MainFixture = Body->CreateFixture(&FixtureDef);

	Body->SetType(bIsDynamic ? b2BodyType::b2_dynamicBody : b2BodyType::b2_kinematicBody);


}


void RigidbodyComponent::OnPostPhysics(float DeltaTime)
{
	GameEntity& Parent = GetParentEntity();
	b2Vec2 BodyPos = Body->GetPosition();
	Parent.GetTransform().Position = glm::vec3(BodyPos.x, BodyPos.y, Parent.GetTransform().Position.z);
	glm::vec3 Euler = Parent.GetTransform().GetEulerRotation();
	Parent.GetTransform().SetEulerRotation(glm::vec3(Euler.x, Euler.y, Body->GetAngle() / 0.0174533f));

	int x, y;
	SDL_GetMouseState(&x, &y);
	glm::vec2 PixelSpace = { x, y };
	glm::vec4 ScreenSpace = glm::vec4(WindowManager::Get().PixelCoordToScreenSpace(PixelSpace), 1.0f);
	glm::mat4 ViewMatrix = CameraComponent::GetActiveCamera()->GetViewMatrix();
	glm::mat4 ProjMatrix = CameraComponent::GetActiveCamera()->GetProjectionMatrix();
	glm::mat4 ViewProjectionMatrix = ProjMatrix * ViewMatrix;
	ViewProjectionMatrix = glm::inverse(ViewProjectionMatrix);
	glm::vec4 WorldSpace = ViewProjectionMatrix * ScreenSpace;
	WorldSpace.z = 0;
	glm::vec3 mouse_pos = WorldSpace;
	glm::vec3 position = GetParentEntity().GetTransform().Position;
	glm::vec2 force = mouse_pos - position;
	Body->ApplyForceToCenter({ force[0], force[1] }, true);
}

void RigidbodyComponent::SetDynamic(bool bDynamic)
{
	if (bSpawned)
	{
		Body->SetType(bDynamic ? b2BodyType::b2_dynamicBody : b2BodyType::b2_kinematicBody);
	}
	bIsDynamic = bDynamic;
	
}

void RigidbodyComponent::OnPostRender(float DeltaTime)
{
	if (!bDrawDebugPolys) return;
	if (!MainFixture) return;

	
}

void RigidbodyComponent::Jump(ActionInfo Info)
{

}

void RigidbodyComponent::Right(ActionInfo Info)
{
}

void RigidbodyComponent::Left(ActionInfo Info)
{
}
