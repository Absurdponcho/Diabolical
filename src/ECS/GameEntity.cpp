#include "GameEntity.h"
#include "../Logging/Logging.h"

void GameEntity::OnTick(float DeltaTime)
{
	GameBaseObject::OnTick(DeltaTime);
	//Logging::LogVerbose("GameEntity::OnTick()", "Ticked");
}

EntityTransform& GameEntity::GetTransform()
{
	return Transform;
}