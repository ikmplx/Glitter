#version 330 core

in VS_OUT {
	vec3 color;
	vec3 pos;
	vec2 uv;

	mat3 tbn;
} vsIn;

uniform samplerCube texture1;

@include ScalarsUbo

layout (location = 0) out vec4 gAlbedoSpecular;
layout (location = 1) out vec3 gNormal;

void main()
{
	vec3 gammaVec = vec3(gamma);

	vec3 cubeColor = pow(texture(texture1, vsIn.color).rgb, gammaVec);

	gAlbedoSpecular = vec4(cubeColor, 0.3);
	gNormal = vsIn.tbn[2];
} 
