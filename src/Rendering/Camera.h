#pragma once
#include "../ECS/GameComponent.h"
#include <glm/glm.hpp>

class CameraComponent : public GameComponent
{
public:
	~CameraComponent();

	void SetActiveCamera();
	float GetOrthographicSize();
	void SetOrthographicSize(float Size);

	// This projection matrix is an orthographic projection matrix.
	// We will have to override it for a perspective matrix if we 
	// ever want to do 3D stuff. But this is fine for this project :)
	virtual glm::mat4x4 GetProjectionMatrix();
	virtual glm::mat4x4 GetViewMatrix();

	void Draw();

	static CameraComponent* GetActiveCamera();

private:

	float OrthographicSize = 10;

	static CameraComponent* ActiveCamera;
};