#version 330 core

in vec2 TexCoord;
out vec4 FragColor; 

uniform sampler2D Texture;

uniform float XCameraOffsetMultiplier;
uniform float YCameraOffsetMultiplier;
uniform vec3 CameraPosition;
uniform float YOffset;
uniform float AspectRatio;
uniform vec2 Scale;

void main()
{
    vec4 TexColor = texture(Texture, (TexCoord + vec2(CameraPosition.x * XCameraOffsetMultiplier, -CameraPosition.y * YCameraOffsetMultiplier + YOffset)) * vec2(AspectRatio, 1) * Scale);
    if (TexColor.a < .5f) discard;
    FragColor = TexColor;
}