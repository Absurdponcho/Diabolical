#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 MVP;
uniform ivec4 SpriteDimensions;
uniform vec2 TextureScale;
uniform float Depth;
out vec2 TexCoord;

void main()
{

	gl_Position = MVP * vec4(aPos, 1.0);
	
	if (Depth != -69420)
	{
		gl_Position.z = Depth;
	}

	int SpriteSheetWidth = SpriteDimensions.z;
	int SpriteSheetHeight = SpriteDimensions.w;
	int SpriteSheetX = SpriteDimensions.x;
	int SpriteSheetY = SpriteDimensions.y;

	vec2 SingleSpriteSize = vec2(aTexCoord.x / SpriteSheetWidth, aTexCoord.y / SpriteSheetHeight);


	vec2 Multiplier = vec2 (1.f / SpriteSheetWidth, 1.f / SpriteSheetHeight);

	TexCoord = vec2(SingleSpriteSize.x + Multiplier.x * SpriteSheetX, SingleSpriteSize.y + Multiplier.y * SpriteSheetY) * TextureScale;
}