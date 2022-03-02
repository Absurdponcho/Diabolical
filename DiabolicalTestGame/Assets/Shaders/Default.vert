#version 330 core
layout (location = 0) in vec3 aPos;
  
uniform mat4 MVP;
out vec3 LocalPos;

void main()
{
    gl_Position = MVP * vec4(aPos, 1.0);
    LocalPos = aPos;
}