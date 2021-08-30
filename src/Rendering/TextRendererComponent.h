#pragma once
#include "GameRendererComponent.h"
#include <string>
#include "FreetypeFontAsset.h"
#include "../Assets/GameAssetSoftPointer.h"

#define TEXT_WINDOW_WIDTH 1280.f
#define TEXT_WINDOW_HEIGHT 720.f

class TextRendererComponent : public GameRendererComponent
{
public:
	TextRendererComponent();
	void SetFont(GameAssetSoftPointer<FreetypeFontAsset> FontAssetPointer);
	virtual void Render(CameraComponent& Camera) override;

	std::string Text = "";
	glm::vec3 ForegroundColor = glm::vec3(1, .5f, .3f);
	glm::vec3 ShadowColor = glm::vec3(1, .5f, .3f);
	float TextSize = 8;
	glm::vec2 ShadowOffset = glm::vec2(0, 0);
	bool bShadow = false;


private:
	void RenderText(CameraComponent& Camera, float x, float y, float scale, glm::vec3 Color);

	FreetypeFontAsset* FontAsset = nullptr;
	static unsigned int VAO, VBO;
	static unsigned int ShaderProgram;
};