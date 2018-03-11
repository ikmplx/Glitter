#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUv;
layout (location = 3) in vec3 aColor;

out VS_OUT {
	vec3 color;
	vec3 normal;
	vec3 pos;
	vec2 uv;
} vsOut;

uniform mat4 model;

@include MatricesUbo

void main()
{
	vec4 pos = vec4(aPos, 1.0);

	gl_Position = combined * model * pos;
	vsOut.pos = (model * pos).xyz;

	mat3 normalMatrix = mat3(transpose(inverse(model)));
	vsOut.normal = normalMatrix * aNormal;

	vsOut.color = aColor;
	vsOut.uv = aUv;
}
