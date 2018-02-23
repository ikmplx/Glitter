#version 330 core

in vec2 vUv;
in vec3 vColor;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;

void main()
{
	vec3 texColor = texture(texture_diffuse1, vUv).rgb;
	FragColor = vec4(texColor * vColor, 1.0);
} 