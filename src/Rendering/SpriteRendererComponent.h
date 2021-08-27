#pragma once
#include "GameRendererComponent.h"
#include "../Assets/GameAssetSoftPointer.h"
#include "../Textures/TextureAsset.h"
#include <string>
#include "glm/glm.hpp"

class SpriteRendererComponent : public GameRendererComponent
{
public:
	virtual void Render(CameraComponent& Camera) override;
	virtual void OnSpawn() override;
	virtual void OnTick(float DeltaTime) override;

	virtual void SelectSpriteSheetIndex(float DeltaTime);

	void SetTexture(GameAssetSoftPointer<TextureAsset>& Texture);

	glm::ivec2 SpriteSheetSize = glm::ivec2(1,1);
	float SpriteSheetProgressionSpeed = 0;
	bool bMirrored = false;
private:
	glm::ivec2 SpriteSheetIndex = glm::ivec2(0, 0);
	float SpriteSheetProgress = 0.f;

	static bool bGLInitialized;
	static unsigned int VertexBufferObject;
	static unsigned int VertexArrayObject;
	static unsigned int ElementBufferObject;
	static unsigned int ShaderProgram;

	unsigned int OpenGLTexture = 0;
};