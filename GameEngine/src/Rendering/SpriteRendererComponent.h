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
	void SetTextureScaling(glm::vec2 Scaling);

	glm::ivec2 SpriteSheetSize = glm::ivec2(1,1);
	float SpriteSheetProgressionSpeed = 0;
	bool bXMirrored = false;
	bool bYMirrored = false;
	bool bUseSpriteLoop = false;
	int SpriteLoopStart = 0;
	int SpriteLoopEnd = 0;
	
	bool bUseYAsDepth = false;

	static void BindVertexArray();
private:
	glm::ivec2 SpriteSheetIndex = glm::ivec2(0, 0);
	glm::vec2 TextureScale = glm::vec2(1, 1);
	float SpriteSheetProgress = 0.f;

	static bool bGLInitialized;
	static unsigned int VertexBufferObject;
	static unsigned int VertexArrayObject;
	static unsigned int ElementBufferObject;
	static unsigned int ShaderProgram;
	static int MVPMatrixLocation;
	static int SpriteDimensionsLocation;
	static int TextureScaleLocation;
	static int DepthLocation;
	unsigned int OpenGLTexture = 0;
};