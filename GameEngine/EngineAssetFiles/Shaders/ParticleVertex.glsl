#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 MVP;
uniform vec4 ParticleColor;
out vec4 VertColor;

void main()
{
   gl_Position = MVP * vec4(aPos, 1.0);
   VertColor = ParticleColor;
}
