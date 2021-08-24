#pragma once
#include "../ECS/GameComponent.h"
#include <glm/glm.hpp>
#include "../Input/InputManager.h"

class CameraComponent : public GameComponent
{
public:
	virtual void OnDestroy() override;

	virtual void OnSpawn() override;

	void MoveUp(ActionInfo& Info);
	void MoveRight(ActionInfo& Info);
	void MoveLeft(ActionInfo& Info);
	void MoveDown(ActionInfo& Info);

	void SetActiveCamera();
	float GetOrthographicSize();
	void SetOrthographicSize(float Size);

	// This projection matrix is an orthographic projection matrix.
	// We will have to override it for a perspective matrix if we 
	// ever want to do 3D stuff. But this is fine for this project :)
	virtual glm::mat4x4 GetProjectionMatrix();
	virtual glm::mat4x4 GetViewMatrix();
	virtual float GetAspectRatio();

	void Draw();

	static CameraComponent* GetActiveCamera();

private:

	float OrthographicSize = 25;

	static CameraComponent* ActiveCamera;
};