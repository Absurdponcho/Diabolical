#include "RigidbodyComponent.h"
#include <box2d/b2_api.h>
#include "PhysicsWorld.h"
#include  <box2d/b2_world.h>
#include "../Utility/Utility.h"
#include "../Logging/Logging.h"


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

	if (b2PolygonShape* Shape = dynamic_cast<b2PolygonShape*>(MainFixture->GetShape()))
	{
		glm::mat4x4 Matrix = GetParentEntity().GetTransRotationMatrix();

		for (int Index = 0; Index < Shape->m_count - 1; Index++)
		{
			glm::vec4 From = glm::vec4(Shape->m_vertices[Index].x, Shape->m_vertices[Index].y, 0, 1);
			glm::vec4 To = glm::vec4(Shape->m_vertices[Index + 1].x, Shape->m_vertices[Index + 1].y, 0, 1);

			From = Matrix * From;
			To = Matrix * To;
			Utility::DrawDebugLine(From, To);
		}

		glm::vec4 From = glm::vec4(Shape->m_vertices[0].x, Shape->m_vertices[0].y, 0, 1);
		glm::vec4 To = glm::vec4(Shape->m_vertices[Shape->m_count - 1].x, Shape->m_vertices[Shape->m_count - 1].y, 0, 1);

		From = Matrix * From;
		To = Matrix * To;

		Utility::DrawDebugLine(From, To);
	}
}
