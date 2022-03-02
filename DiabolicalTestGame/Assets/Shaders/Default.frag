#version 330 core

out vec4 FragColor;
in vec3 LocalPos;

void main()
{
    vec3 Color = vec3(.8, .8, .8);
    float total = floor(LocalPos.x) +
                  floor(LocalPos.y) +
                  floor(LocalPos.z);
    vec3 col = vec3(.3, .3, .3);
    FragColor = vec4(col * mod(total, 2.0) + vec3(.5, .5, .5),1);
}