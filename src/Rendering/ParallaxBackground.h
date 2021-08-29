#pragma once
#include <vector>
#include <glm/glm.hpp>

class TextureAsset;
class CameraComponent;

struct BackgroundContainer
{
	TextureAsset* Texture;
	float RenderDepth = 0.99999f;
	float XCameraOffsetMultiplier = 0;
	float YOffset = 0;
	glm::vec2 Scale = glm::vec2(1, 1);
};
class ParallaxBackground
{
public:
	void Draw(CameraComponent* Camera);
	void Initialize();
	static void InitializeShader();
private:
	bool bInitialized = false;
	static unsigned int ShaderProgram;
	std::vector<BackgroundContainer> BackgroundOrder;
	static unsigned int VertexBufferObject;
	static unsigned int VertexArrayObject;
	static unsigned int ElementBufferObject;
};