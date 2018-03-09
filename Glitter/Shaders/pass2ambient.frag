#version 330 core

in vec2 vUv;
out vec4 FragColor;

uniform sampler2D gAlbedoSpecular;

struct AmbientLight
{
	vec3 color;
};
uniform AmbientLight ambientLight;

void main()
{
	vec3 texColor = texture(gAlbedoSpecular, vUv).rgb;
	vec3 ambient = texColor * ambientLight.color;
	FragColor = vec4(ambient, 1.0);
} 
