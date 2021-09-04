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
	virtual void OnDestroy() override;
	virtual void OnTick(float DeltaTime) override;

	virtual void SelectSpriteSheetIndex(float DeltaTime);

	void SetTexture(GameAssetSoftPointer<TextureAsset>& Texture);

	glm::ivec2 SpriteSheetSize = glm::ivec2(1,1);
	float SpriteSheetProgressionSpeed = 0;
	bool bXMirrored = false;
	bool bYMirrored = false;
	bool bUseSpriteLoop = false;
	int SpriteLoopStart = 0;
	int SpriteLoopEnd = 0;

	static void BindVertexArray();
private:
	glm::ivec2 SpriteSheetIndex = glm::ivec2(0, 0);
	float SpriteSheetProgress = 0.f;

	static bool bGLInitialized;
	static unsigned int VertexBufferObject;
	static unsigned int VertexArrayObject;
	static unsigned int ElementBufferObject;
	static unsigned int ShaderProgram;
	static int MVPMatrixLocation;
	static int SpriteDimensionsLocation;
	unsigned int OpenGLTexture = 0;
};