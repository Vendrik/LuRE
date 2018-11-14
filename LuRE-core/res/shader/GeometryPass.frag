#version 330 core
layout (location = 0) out vec3 gNormal;
layout (location = 1) out vec4 gAlbedoSpec;

in vec3 Normal;

uniform vec3 objectColor = vec3(0.4, 0.5, 1.9);

void main()
{    
    // store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);
    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = objectColor;
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = 0.5;
    
}  