#include <box2d/b2_api.h>
#include "PhysicsWorld.h"
#include  <box2d/b2_world.h>
#include "../Utility/Utility.h"
#include "../Logging/Logging.h"
#include "RigidbodyComponent.h"
#include "..\WindowManager.h"
#include "..\Rendering\Camera.h"

std::unordered_map<b2Body*, RigidbodyComponent*> RigidbodyComponent::BodyRigidbodyMap;

void RigidbodyComponent::OnSpawn()
{
	bSpawned = true;

	GameEntity* Parent = GetParentEntity();
	b2BodyDef BodyDef;
	BodyDef.type = b2BodyType::b2_dynamicBody;
	BodyDef.position.Set(Parent->GetTransform().GetPosition().x, Parent->GetTransform().GetPosition().y);
	BodyDef.angle = Parent->GetTransform().GetEulerRotation().z * 0.0174533f; // deg to rad

	Body = PhysicsWorld::Get().GetWorld().CreateBody(&BodyDef);

	Body->SetType(bIsDynamic ? b2BodyType::b2_dynamicBody : b2BodyType::b2_staticBody);
	Body->SetFixedRotation(!bIsRotating);
	Body->SetLinearDamping(DesiredLinearDamping);
	Body->SetLinearVelocity(DesiredVelocity);
	Body->SetGravityScale(bUsesGravity ? 1.f : 0.f);

	BodyRigidbodyMap[Body] = this;
}

void RigidbodyComponent::OnDestroy()
{
	Check(Body);
	BodyRigidbodyMap.erase(Body);

	PhysicsWorld::Get().GetWorld().DestroyBody(Body);
	Body = nullptr;
}

void RigidbodyComponent::OnPostPhysics(float FixedDeltaTime)
{
	GameComponent::OnPostPhysics(FixedDeltaTime);

	// perform horizontal damping
	b2Vec2 Velocity = GetVelocity();
	Velocity = b2Vec2(Velocity.x - (Velocity.x * FixedDeltaTime * HorizontalDamping), Velocity.y);
	SetVelocity(Velocity);
	//!perform horizontal damping
	
	GameEntity* Parent = GetParentEntity();
	b2Vec2 BodyPos = Body->GetPosition();
	Parent->GetTransform().RigidbodySetPosition(b2Vec2(BodyPos.x + Offset.x, BodyPos.y + Offset.y));
	glm::vec3 Euler = Parent->GetTransform().GetEulerRotation();
	Parent->GetTransform().SetEulerRotation(glm::vec3(Euler.x, Euler.y, Body->GetAngle() / 0.0174533f));

	/*int x, y;
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
	glm::vec3 position = GetParentEntity()->GetTransform().Position;
	glm::vec2 force = mouse_pos - position;*/
	//Body->ApplyForceToCenter({ force[0], force[1] }, true);
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
	if (bSpawned)
	{
		Body->SetLinearVelocity(Velocity);
		return;
	}
	DesiredVelocity = Velocity;
}

b2Vec2 RigidbodyComponent::GetVelocity()
{
	if (!bSpawned)
	{
		return DesiredVelocity;
	}
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

b2Body* RigidbodyComponent::GetBody()
{
	return Body;
}

void RigidbodyComponent::SetOffset(glm::vec2 NewOffset)
{
	Offset = NewOffset;
}

void RigidbodyComponent::SetUsesGravity(bool bGravity)
{
	bUsesGravity = bGravity;
	if (Body)
	{
		Body->SetGravityScale(bUsesGravity ? 1.f : 0.f);
	}
}

void RigidbodyComponent::SetPosition(b2Vec2 Position)
{
	Check(bSpawned); // need to fix so that we can set position before spawned

	Check(Body);

	Body->SetTransform(Position, Body->GetAngle());
}

glm::vec2 RigidbodyComponent::GetOffset()
{
	return Offset;
}