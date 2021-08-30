#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    vec4 Pos = projection * vec4(vertex.xy, 0, 1.0);
    gl_Position = vec4(Pos.x, Pos.y, -.9999, Pos.w);
    TexCoords = vertex.zw;
}  