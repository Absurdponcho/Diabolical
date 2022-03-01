#version 330 core
layout (location = 0) in vec3 aPos;
  
uniform mat4 MVP;
uniform vec3 Color;
out vec4 vertexColor;

void main()
{
    gl_Position = MVP * vec4(aPos, 1.0);
    
    vertexColor = vec4(Color, 1.0);
}