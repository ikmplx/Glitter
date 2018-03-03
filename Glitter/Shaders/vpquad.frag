#version 330 core

in vec2 vUv;
out vec4 FragColor;

uniform sampler2D texture1;

void main()
{
	vec4 texColor = texture(texture1, vUv);
	FragColor = vec4(texColor.rgb, 1.0);
} 
