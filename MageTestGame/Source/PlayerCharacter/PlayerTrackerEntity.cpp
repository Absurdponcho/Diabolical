#include "PlayerTrackerEntity.h"

void PlayerTrackerEntity::SetTrackTarget(GameEntity* TrackTarget)
{
	Check(TrackTarget);
	TrackTargetWeakPtr = GameWeakObjectPointer<GameEntity>(TrackTarget);
}
void PlayerTrackerEntity::OnTick(float DeltaTime)
{
	GameEntity::OnTick(DeltaTime);

	if (GameEntity* Entity = TrackTargetWeakPtr.Get())
	{
		glm::vec3 Position = GetTransform().GetPosition();
		glm::vec3 Difference = Entity->GetTransform().GetPosition() - Position;
		
		GetTransform().SetPosition(GetTransform().GetPosition() + (Difference * DeltaTime * TrackSpeed));
	}

	EntityTransform& Transform = GetTransform();
	glm::vec3 NewPosition = Transform.GetPosition();

	if (NewPosition.x < Bounds.x) NewPosition.x = Bounds.x;
	if (NewPosition.y < Bounds.y) NewPosition.y = Bounds.y;
	if (NewPosition.x > Bounds.z) NewPosition.x = Bounds.z;
	if (NewPosition.y > Bounds.w) NewPosition.y = Bounds.w;


	Transform.SetPosition(NewPosition);
}

void PlayerTrackerEntity::LockToBoundary(bool bLocked)
{
	bLockedToBoundary = bLocked;
}

void PlayerTrackerEntity::SetBoundary(glm::vec2 Min, glm::vec2 Max)
{
	Bounds = glm::vec4(Min.x, Min.y, Max.x, Max.y);
}