#version 330 core

in vec2 vUv;
out vec4 FragColor;

uniform sampler2D texture1;
uniform float gamma;

void main()
{
	vec3 texColor = texture(texture1, vUv).rgb;
	FragColor = vec4(pow(texColor, vec3(1.0 / gamma)), 1.0);
} 
