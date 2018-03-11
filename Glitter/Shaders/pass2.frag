#version 330 core

in vec2 vUv;
out vec4 FragColor;

uniform sampler2D gAlbedoSpecular;
uniform sampler2D gNormal;

void main()
{
	vec3 normal = texture(gNormal, vUv).xyz;
	vec4 texColor = texture(gAlbedoSpecular, vUv);

	vec3 albedo = texColor.rgb;
	float spec = texColor.a;

	FragColor = vec4(texColor.rgb, 1.0);
} 
