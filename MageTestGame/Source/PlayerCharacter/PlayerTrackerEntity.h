#pragma once
#include <ECS/GameEntity.h>
#include <ECS/GameWeakObjectPointer.h>

class PlayerTrackerEntity : public GameEntity
{
public:
	PlayerTrackerEntity() {}
	void SetTrackTarget(GameEntity* TrackTarget);
	virtual void OnTick(float DeltaTime) override;

	float TrackSpeed = 3;

	void LockToBoundary(bool bLocked);
	void SetBoundary(glm::vec2 Min, glm::vec2 Max);

private:
	bool bLockedToBoundary = false;
	glm::vec4 Bounds = glm::vec4(-20, -20, 20, 20);
	GameWeakObjectPointer<GameEntity> TrackTargetWeakPtr;
};