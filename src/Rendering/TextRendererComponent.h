#pragma once
#include "GameRendererComponent.h"
#include <string>
#include "FreetypeFontAsset.h"
#include "../Assets/GameAssetSoftPointer.h"

class TextRendererComponent : public GameRendererComponent
{
public:
	TextRendererComponent();
	void SetFont(GameAssetSoftPointer<FreetypeFontAsset> FontAssetPointer);
	virtual void Render(CameraComponent& Camera) override;
	void RenderText(float x, float y, float scale);

	std::string Text = "";
	glm::vec3 Color = glm::vec3(1, .5f, .3f);

private:
	FreetypeFontAsset* FontAsset = nullptr;
	static unsigned int VAO, VBO;
	static unsigned int ShaderProgram;
};