#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUvs;
layout (location = 2) in vec3 aNormal;

layout (location = 3) in mat4 instanceOffset;

out vec3 Normal;

uniform mat4 model;
uniform mat4 view = mat4(1.0);
uniform mat4 projection = mat4(1.0);

void main()
{
    vec3 FragPos = vec3(view * model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(view * model))) * aNormal;  
    
    gl_Position = projection * vec4(FragPos, 1.0);
}