#version 330 core

in vec3 vUvw;
in vec3 vDir;

out vec4 FragColor;

uniform samplerCube sampler;
uniform float gamma;

uniform vec3 sunDir;

void main()
{
	FragColor = texture(sampler, vUvw);
	FragColor.rgb = pow(FragColor.rgb, vec3(gamma));

	float sunDist = length(normalize(vDir) - sunDir);

	float sunValue = 1.0 - smoothstep(0.02, 0.04, sunDist);
	FragColor.rgb += sunValue;

	float haloValue = pow(1.0 - smoothstep(0.15, 1.0, sunDist), 16.0);
	FragColor.rgb += haloValue * 0.04;
} 
