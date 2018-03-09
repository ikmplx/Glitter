#version 330 core

in vec2 vUv;
out vec4 FragColor;

uniform sampler2D gAlbedoSpecular;
uniform sampler2D gNormal;
uniform sampler2D gPosition;

struct DirLight
{
	vec3 color;
	vec3 direction;
	float ambient;
};
uniform DirLight dirLight;

uniform int enableBlinn = 0;

layout (std140) uniform Vectors
{
	vec3 cameraPos;
};

void main()
{
	vec3 normal = normalize(texture(gNormal, vUv).xyz);
	vec4 texColor = texture(gAlbedoSpecular, vUv);
	vec3 position = texture(gPosition, vUv).xyz;

	vec3 albedo = texColor.rgb;
	float spec = texColor.a;
	float shininess = 32.0;

	vec3 lightDir = normalize(-dirLight.direction);

	// Ambient component of dir light
	vec3 ambient = albedo * dirLight.color * dirLight.ambient;
	
	// Diffuse
	float diffuseIntensity = max(0.0, dot(normal, lightDir));
	vec3 diffuse = albedo * dirLight.color * diffuseIntensity;
	
	// Specular
	vec3 viewDir = normalize(cameraPos - position);
	float specularIntencity = 0.0;

	if (enableBlinn == 0) {
		vec3 reflectDir = reflect(-lightDir, normal);
		specularIntencity = pow(max(0.0, dot(viewDir, reflectDir)), shininess);
	} else {
		vec3 halfwayDir = normalize(lightDir + viewDir);
		specularIntencity = pow(max(0.0, dot(normal, halfwayDir)), shininess * 3.5);
	}
	vec3 specular = vec3(spec * specularIntencity) * dirLight.color;
	
	FragColor = vec4(diffuse + ambient + specular, 1.0);
} 
