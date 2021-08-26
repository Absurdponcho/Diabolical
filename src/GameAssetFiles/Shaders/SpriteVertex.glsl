#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 MVP;
out vec4 VertColor;

void main()
{
   gl_Position = MVP * vec4(aPos, 1.0);
   VertColor = vec4(sin(gl_Position.x * 50), sin(gl_Position.y * 50), sin(gl_Position.z * 50), 1.0);
}