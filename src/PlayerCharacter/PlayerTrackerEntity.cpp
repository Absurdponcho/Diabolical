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
		glm::vec3 Position = GetTransform().Position;
		glm::vec3 Difference = Entity->GetTransform().Position - Position;
		
		GetTransform().Position += Difference * DeltaTime * TrackSpeed;
	}
}