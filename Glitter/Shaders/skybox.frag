#version 330 core

in vec3 vUvw;

out vec4 FragColor;

uniform samplerCube sampler;
uniform float gamma;

void main()
{
	FragColor = texture(sampler, vUvw);
	FragColor.rgb = pow(FragColor.rgb, vec3(gamma));
} 
