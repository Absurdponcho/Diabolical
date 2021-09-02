#version 330 core

in vec2 TexCoord;
out vec4 FragColor; 

uniform sampler2D SpriteTexture;

void main()
{
    vec4 TexColor = texture(SpriteTexture, TexCoord);
    if (TexColor.a < .5f) discard;
    FragColor = TexColor;
}