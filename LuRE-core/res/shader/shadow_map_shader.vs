#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 lightView;
uniform mat4 projection;

void main()
{
    gl_Position = projection * lightView * model * vec4(aPos, 1.0);
}