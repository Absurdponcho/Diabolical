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
}