#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUv;
layout (location = 3) in vec3 aColor;

out vec2 vUv;
out vec3 vColor;

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

	vUv = aUv;
	vColor = aColor;
}