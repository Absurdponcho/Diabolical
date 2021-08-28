#pragma once
#include <gl/glew.h>
class Framebuffer;

class RenderPipeline
{
public:
	// Bind a render pipeline to set it as active. Binding a bound render pipeline
	// will reset it and reset all framebuffers and stuff
	virtual void Bind();
	virtual void Unbind();
	virtual void Draw(float DeltaTime);

	static RenderPipeline* GetActive();
private:
	static RenderPipeline* ActiveRenderPipeline;
	Framebuffer* MainFramebuffer;

};