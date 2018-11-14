#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUvs;
layout (location = 2) in vec3 aNormal;

layout (location = 3) in mat4 instanceOffset;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view = mat4(1.0);
uniform mat4 projection = mat4(1.0);

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}