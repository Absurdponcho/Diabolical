#pragma once
#include <gl/glew.h>
#include <gl/GL.h>

#include "../Assets/TextAsset.h"
#include "../Assets/GameAssetSoftPointer.h"

class ShaderCompiler
{
public:
	static GLuint CompileShaderProgram(GameAssetSoftPointer<TextAsset> VertexShader, GameAssetSoftPointer<TextAsset> FragmentShader);
};