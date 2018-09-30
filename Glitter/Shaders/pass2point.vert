#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aUv;

noperspective out vec2 vUv;

uniform mat4 model;

@include MatricesUbo

void main()
{
	vec4 pos = vec4(aPos, 1.0);

	vec4 glPosition = combined * model * pos;
	gl_Position = glPosition;

	vUv = vec2((glPosition.x / glPosition.w + 1.0) * 0.5, (glPosition.y / glPosition.w + 1.0) * 0.5);
}
