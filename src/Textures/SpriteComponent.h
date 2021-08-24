#pragma once
#include "TextureAsset.h"
#include "glm/glm.hpp"
#include "gl/glew.h"
#include "..\ECS\GameComponent.h"

class SpriteComponent : public GameComponent
{
public:
	virtual void OnSpawn() override;

private:
	std::string file;

	glm::vec3 Position; // x y and zindex
	float Alpha;
	bool bVisible;

	// unused for now until static sprite working
	bool bIsText; // if is text load font file and read SpriteComponent::Text for input
	std::string Text;
	unsigned int fontSize;
};

