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
	Body->SetFixedRotation(!bIsRotating);
	Body->SetLinearDamping(DesiredLinearDamping);


}


void RigidbodyComponent::OnPostPhysics(float DeltaTime)
{
	// perform horizontal damping
	b2Vec2 Velocity = GetVelocity();
	Velocity = b2Vec2(Velocity.x - (Velocity.x * DeltaTime), Velocity.y);
	SetVelocity(Velocity);
	//!perform horizontal damping

	GameEntity& Parent = GetParentEntity();
	b2Vec2 BodyPos = Body->GetPosition();
	Parent.GetTransform().Position = glm::vec3(BodyPos.x, BodyPos.y, Parent.GetTransform().Position.z);

	glm::vec3 Euler = Parent.GetTransform().GetEulerRotation();
	Parent.GetTransform().SetEulerRotation(glm::vec3(Euler.x, Euler.y, Body->GetAngle() / 0.0174533f));
}

void RigidbodyComponent::SetDynamic(bool bDynamic)
{
	if (bSpawned)
	{
		Body->SetType(bDynamic ? b2BodyType::b2_dynamicBody : b2BodyType::b2_kinematicBody);
	}
	bIsDynamic = bDynamic;
	
}

void RigidbodyComponent::SetRotates(bool bRotates)
{
	if (bSpawned)
	{
		Body->SetFixedRotation(!bRotates);
	}
	bIsRotating = bRotates;
}

void RigidbodyComponent::SetLinearDamping(float Drag)
{
	if (bSpawned)
	{
		Body->SetLinearDamping(Drag);
	}
	DesiredLinearDamping = Drag;
}

void RigidbodyComponent::SetVelocity(b2Vec2 Velocity)
{
	Body->SetLinearVelocity(Velocity);
}

b2Vec2 RigidbodyComponent::GetVelocity()
{
	return Body->GetLinearVelocity();
}

void RigidbodyComponent::SetHorizonalDamping(float Damping)
{
	HorizontalDamping = Damping;
}

void RigidbodyComponent::OnPostRender(float DeltaTime)
{
	if (!bDrawDebugPolys) return;
	if (!MainFixture) return;

	
}


void RigidbodyComponent::Jump(ActionInfo Info)
{
	if (Info.EventType == EEventType::MouseButtonEvent) {
		b2Vec2 world = Body->GetWorldCenter();
		glm::vec2 PixelSpace = { Info._event.MouseButtonEvent->x, Info._event.MouseButtonEvent->y };
		glm::vec4 ScreenSpace = glm::vec4(WindowManager::Get().PixelCoordToScreenSpace(PixelSpace), 1.0f);
		glm::mat4 ViewMatrix = CameraComponent::GetActiveCamera()->GetViewMatrix();
		glm::mat4 ProjMatrix = CameraComponent::GetActiveCamera()->GetProjectionMatrix();
		glm::mat4 ViewProjectionMatrix = ProjMatrix * ViewMatrix;
		ViewProjectionMatrix = glm::inverse(ViewProjectionMatrix);
		glm::vec4 WorldSpace = ViewProjectionMatrix * ScreenSpace;
		WorldSpace.z = 0;
		glm::vec3 ws = WorldSpace;
		if (glm::distance(ws, GetParentEntity().GetTransform().Position) < 1.0f) {
			Body->ApplyForce({ 0,100 }, world, true);
		}
	}

}

void RigidbodyComponent::Right(ActionInfo Info)
{
}

void RigidbodyComponent::Left(ActionInfo Info)
{
}

void RigidbodyComponent::AddForceAtCenter(const b2Vec2& Force)
{
	if (Body)
	{
		Body->ApplyForceToCenter(Force, true);
	}
}