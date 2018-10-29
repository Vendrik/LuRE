#version 330 core
#define MAX_LIGHTS 32

out vec4 FragColor;
  
in vec2 TexCoords;
//in vec4 FragPosLightSpace;

uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gDepth;
//uniform sampler2D gShadowMap;

uniform ivec4 viewport;
uniform vec2 depth_range;

uniform mat4 LigthSpaceMatrix;
uniform mat4 ViewMatrix;
uniform mat4 PrjMatrix;
uniform mat4 InvPrjMatrix;
uniform vec3 lightColor;
uniform vec3 lightPos; 

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


/* From: https://www.khronos.org/opengl/wiki/Compute_eye_space_from_window_space */
vec4 CalcEyeFromWindow(in vec3 windowSpace)
{
    vec3 ndcPos;
    ndcPos.xy = ((2.0 * windowSpace.xy) - (2.0 * viewport.xy)) / (viewport.zw) - 1.0;
    ndcPos.z = (2.0 * windowSpace.z - depth_range.x - depth_range.y) / (depth_range.y - depth_range.x); // windowSpace.z; 

    vec4 clipPos;
    clipPos.w = PrjMatrix[3][2] / (ndcPos.z - (PrjMatrix[2][2] / PrjMatrix[2][3]));
    clipPos.xyz = ndcPos * clipPos.w;

    return InvPrjMatrix * clipPos;
}

/*
float ShadowCalculation(vec4 pos)
{

    vec4 FragPosLightSpace = LigthSpaceMatrix * pos;


    // perform perspective divide
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;

    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0)
        return 0.0;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(gShadowMap, projCoords.xy).r; 

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // check whether current frag pos is in shadow
    float shadow = currentDepth - 0.05 > closestDepth  ? 1.0 : 0.0;

    return shadow;
}  
*/

void main()
{        
	const float gamma = 2.2;
    
	// retrieve data from G-buffer
    vec4 temp = CalcEyeFromWindow(vec3(gl_FragCoord.x, gl_FragCoord.y, texture(gDepth, TexCoords).r));
    vec3 FragPos = vec3(temp / temp.w);

    vec4 world_space = inverse(ViewMatrix) * vec4(FragPos, 1.0);


    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    //float ShadowMap = texture(gShadowMap, TexCoords).r;
    

    if (Normal == vec3(0.0, 0.0, 0.0))
    {
        FragColor = vec4(0.2, 0.3, 0.3, 1.0);
    }

    else 
    {
		vec3 result = vec3(0.0, 0.0, 0.0);
        
        //ambient
        result = 0.1 * Albedo;

		for (int i = 0; i < lights_count; i++)
		{
			//diffuse
			vec3 lightDir = normalize(vec3( ViewMatrix * l[i].position) - FragPos); 
			vec3 diffuse = max(dot(Normal, lightDir), 0.1) * l[i].color.rgb;

			//specular
			vec3 viewDir = normalize(-FragPos);
			vec3 reflectDir = reflect(-lightDir, Normal);  
			vec3 specular = Specular * pow(max(dot(viewDir, reflectDir), 0.0), 64) * l[i].color.rgb; 

			result += (diffuse + specular) * Albedo;
		}

		// Exposure tone mapping
		vec3 mapped = vec3(1.0) - exp(-result * exposure);

        FragColor = vec4(mapped , 1.0);
    }

}
