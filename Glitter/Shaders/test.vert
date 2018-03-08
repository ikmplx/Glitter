#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUv;
layout (location = 3) in vec3 aColor;

out VS_OUT {
	vec2 uv;
	vec3 color;
	vec3 pos;
} vsOut;

uniform mat4 model;

layout (std140) uniform Matrices
{
    mat4 proj;
    mat4 view;
	mat4 combined;
};

void main()
{
	gl_Position = combined * model * vec4(aPos, 1.0);

	vsOut.uv = aUv;
	vsOut.color = aColor;

	//vsOut.color = (mat4(mat3(model)) * vec4(aNormal, 1.0)).xyz;
	//vsOut.color = vsOut.color * 0.5 + 0.5;
}