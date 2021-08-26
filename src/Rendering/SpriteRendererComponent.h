#pragma once
#include "GameRendererComponent.h"
#include <string>

class SpriteRendererComponent : public GameRendererComponent
{
public:
	virtual void Render(CameraComponent& Camera) override;
	virtual void OnSpawn() override;

private:
	static bool bGLInitialized;
	static unsigned int VertexBufferObject;
	static unsigned int VertexArrayObject;
	static unsigned int ElementBufferObject;
	static unsigned int ShaderProgram;

};