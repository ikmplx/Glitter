#version 330 core

noperspective in vec2 vUv;
out vec4 FragColor;

uniform sampler2D gAlbedoSpecular;
uniform sampler2D gNormal;
uniform sampler2D gDepth;

@include VectorsUbo
@include MatricesExtUbo

@include Utils

struct PointLight
{
	vec3 position;
	vec3 color;
	float constant;
	float linear;
	float quadratic;
};
uniform PointLight pointLight;

uniform int enableBlinn = 0;

void main()
{
	//vec2 uv = gl_FragCoord.xy / vec2(1280, 800);
	vec2 uv = vUv;

	vec3 normal = normalize(texture(gNormal, uv).xyz);
	vec4 texColor = texture(gAlbedoSpecular, uv);
	float depth = texture(gDepth, uv).r;
	vec3 position = DepthToPosition(depth, uv);

	vec3 albedo = texColor.rgb;
	float spec = texColor.a;
	float shininess = 32.0;

	vec3 lightDir = normalize(pointLight.position - position);

	float lightDist = length(pointLight.position - position);
	float attenuation = 1.0 / (pointLight.constant + pointLight.linear * lightDist + pointLight.quadratic * lightDist * lightDist);

	// Diffuse
	float diffuseIntensity = max(0.0, dot(normal, lightDir)) * attenuation;
	vec3 diffuse = albedo * pointLight.color * diffuseIntensity;
	
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
	specularIntencity *= attenuation;
	vec3 specular = spec * pointLight.color * specularIntencity;
	
	FragColor = vec4(diffuse + specular, 1.0);
} 
