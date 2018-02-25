#version 330 core

in vec3 vUvw;

out vec4 FragColor;

uniform samplerCube sampler;

void main()
{
	//FragColor = vec4(vUvw, 1.0);
	FragColor = texture(sampler, vUvw);
} 
