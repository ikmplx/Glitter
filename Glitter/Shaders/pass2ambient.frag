#version 330 core

in vec2 vUv;
out vec4 FragColor;

uniform sampler2D gAlbedoSpecular;

struct AmbientLight
{
	vec3 color;
};
uniform AmbientLight ambientLight;

uniform float gamma;

void main()
{
	vec3 texColor = pow(texture(gAlbedoSpecular, vUv).rgb, vec3(gamma));
	vec3 ambient = texColor * ambientLight.color;
	
	FragColor = vec4(ambient, 1.0);
} 
