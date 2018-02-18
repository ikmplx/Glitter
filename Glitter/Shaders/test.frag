#version 330 core

in vec2 vUv;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;

void main()
{
	vec4 texColor = texture(texture_diffuse1, vUv);
	FragColor = texColor;
} 