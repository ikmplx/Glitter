#version 330 core

in vec2 vUv;
in vec3 vColor;

out vec4 FragColor;

uniform bool has_diffuse1;
uniform sampler2D texture_diffuse1;

void main()
{
	vec3 texColor = vColor;

	if (has_diffuse1) {
		texColor *= texture(texture_diffuse1, vUv).rgb;
	}
	
	FragColor = vec4(texColor, 1.0);
} 