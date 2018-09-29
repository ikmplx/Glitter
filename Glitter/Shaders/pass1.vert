#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUv;
layout (location = 3) in vec3 aColor;
layout (location = 4) in vec3 aTangent;
layout (location = 5) in vec3 aBitangent;

out VS_OUT {
	vec3 color;
	vec3 pos;
	vec2 uv;

	mat3 tbn;
} vsOut;

uniform mat4 model;

@include MatricesUbo

void main()
{
	vec4 pos = vec4(aPos, 1.0);

	gl_Position = combined * model * pos;
	vsOut.pos = (model * pos).xyz;

	/*
	mat3 normalMatrix = mat3(transpose(inverse(model)));
	vsOut.normal = normalMatrix * aNormal;
	*/

	vec3 T = normalize(vec3(model * vec4(aBitangent, 0.0)));
	vec3 B = normalize(vec3(model * vec4(aTangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));

	vsOut.tbn = mat3(T, B, N);

	vsOut.color = aColor;
	vsOut.uv = aUv;
}
