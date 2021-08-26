#pragma once
#include "GameRendererComponent.h"
#include "../Assets/GameAssetSoftPointer.h"
#include "../Textures/TextureAsset.h"
#include <string>

class SpriteRendererComponent : public GameRendererComponent
{
public:
	virtual void Render(CameraComponent& Camera) override;
	virtual void OnSpawn() override;

	void SetTexture(GameAssetSoftPointer<TextureAsset>& Texture);

private:
	static bool bGLInitialized;
	static unsigned int VertexBufferObject;
	static unsigned int VertexArrayObject;
	static unsigned int ElementBufferObject;
	static unsigned int ShaderProgram;

	unsigned int OpenGLTexture = 0;
};