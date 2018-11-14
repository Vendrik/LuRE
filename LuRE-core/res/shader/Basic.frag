#version 330 core
#define MAX_LIGHTS 32

out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform float exposure = 1.0;
uniform int lights_count = 0;

struct Light
{
	vec4 position;
	vec4 color;
};

// Uniform Buffer Object
layout (std140) uniform Lights
{
	Light l[MAX_LIGHTS];
};

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * objectColor;

	vec3 result = ambient;

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	float specularStrength = 0.5;
	

	for (int i = 0; i < lights_count; i++)
	{
		// diffuse 
		vec3 lightDir = normalize(l[i].position.xyz - FragPos);  	

		float diff = max(dot(norm, lightDir), 0.1);
		vec3 diffuse = diff * l[i].color.rgb * objectColor;
    
		// specular
		vec3 reflectDir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
		vec3 specular = specularStrength * spec * l[i].color.rgb * objectColor;  

		result += diffuse + specular;
	}
	
	// Exposure tone mapping
	result = vec3(1.0) - exp(-result * exposure);

    FragColor = vec4(result, 1.0);
} 