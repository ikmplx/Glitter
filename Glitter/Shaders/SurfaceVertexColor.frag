#version 330 core

in VS_OUT {
	vec3 color;
	vec3 normal;
	vec3 pos;
	vec2 uv;
} vsIn;

@include ScalarsUbo

layout (location = 0) out vec4 gAlbedoSpecular;
layout (location = 1) out vec3 gNormal;

void main()
{
	vec3 gammaVec = vec3(gamma);
	vec3 vertexColor = pow(vsIn.color, gammaVec);

	float spec = 0.5;
	
	gAlbedoSpecular = vec4(vertexColor, spec);
	gNormal = vsIn.normal;
} 
