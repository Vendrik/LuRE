#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUvs;

out vec2 TexCoords;
//out vec4 FragPosLightSpace;

//uniform mat4 LigthSpaceMatrix;

void main()
{
	TexCoords = aUvs;
	//FragPosLightSpace = LigthSpaceMatrix * aPos;

	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}