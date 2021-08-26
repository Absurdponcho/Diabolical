#pragma once
#include "../ECS/GameEntity.h"
#include "../ECS/GameWeakObjectPointer.h"

class PlayerTrackerEntity : public GameEntity
{
public:
	PlayerTrackerEntity() {}
	void SetTrackTarget(GameEntity* TrackTarget);
	virtual void OnTick(float DeltaTime) override;

	float TrackSpeed = 3;

private:
	GameWeakObjectPointer<GameEntity> TrackTargetWeakPtr;
};