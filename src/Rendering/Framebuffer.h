#pragma once
#include <gl/glew.h>

class Framebuffer
{
public:
	Framebuffer(int NewWidth, int NewHeight);
	void Bind();

	// Draws this framebuffer's contents on to the bound framebuffer
	void Draw();

	void Clear();
	static GLuint GetVAO();

	static void Unbind();
private:
	static void Initialize();

	int Width = 0;
	int Height = 0;
	GLuint FramebufferName = 0;
	GLuint RenderedTexture = 0;
	GLuint DepthRenderBuffer = 0;
	static GLuint ShaderProgram;
	static GLuint VertexBufferObject;
	static GLuint VertexArrayObject;
	static GLuint ElementBufferObject;
};