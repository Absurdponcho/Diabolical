#pragma once
#include "Types/DString.h"
#include "gl/glew.h"
#include <gl/GL.h>
#include "Types/DMemory.h"

class DMaterial
{
public:
	void BuildShader(const DString& Vertex, const DString& Fragment);
	void DestroyShader();
	const GLuint GetProgram() { return ShaderProgram; }

	~DMaterial();

	static DSharedPtr<DMaterial> DefaultMaterial;
	static void InitializeDefaultMaterial();
protected:
	void BuildVertexShader(const DString& Vertex);
	void BuildFragmentShader(const DString& Fragment);
	void BuildProgram();

	GLuint VertexShader = 0;
	GLuint FragmentShader = 0;
	GLuint ShaderProgram = 0;
};

