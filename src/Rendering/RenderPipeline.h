#pragma once
#include <gl/glew.h>
#include "ParallaxBackground.h"
class Framebuffer;

class RenderPipeline
{
public:
	// Bind a render pipeline to set it as active. Binding a bound render pipeline
	// will reset it and reset all framebuffers and stuff
	virtual void Bind();
	virtual void Unbind();
	virtual void Draw(CameraComponent* Camera, float DeltaTime);
	RenderPipeline();

	static RenderPipeline* GetActive();
private:
	static RenderPipeline* ActiveRenderPipeline;
	Framebuffer* MainFramebuffer = nullptr;
	ParallaxBackground BackgroundContainer;
};