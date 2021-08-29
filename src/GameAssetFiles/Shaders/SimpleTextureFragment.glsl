#version 330 core

in vec2 TexCoord;

out vec4 FragColor; 

uniform sampler2D Texture;
uniform float Time;

void main()
{
    vec2 WobblyTexCoord = vec2(TexCoord.x + sin(TexCoord.x + log(Time)* 55) / 20, TexCoord.y + cos(TexCoord.y + Time * 55) / 20);
    FragColor = vec4(texture(Texture, WobblyTexCoord).xyz, 1);
}