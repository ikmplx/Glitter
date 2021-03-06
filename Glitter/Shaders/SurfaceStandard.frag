#version 330 core

in VS_OUT {
	vec3 color;
	vec3 pos;
	vec2 uv;

	mat3 tbn;
} vsIn;

uniform bool hasDiffuse1;
uniform sampler2D textureDiffuse1;

uniform bool hasSpecular1;
uniform sampler2D textureSpecular1;

uniform bool hasNormal1;
uniform sampler2D textureNormal1;

uniform float specularBase;

@include ScalarsUbo

layout (location = 0) out vec4 gAlbedoSpecular;
layout (location = 1) out vec3 gNormal;

void main()
{
	vec3 gammaVec = vec3(gamma);

	vec3 texColor = pow(vsIn.color, gammaVec);
	float spec = specularBase;

	if (hasDiffuse1) {
		texColor *= pow(texture(textureDiffuse1, vsIn.uv).rgb, gammaVec);
	}
	
	if (hasSpecular1) {
		vec3 specVec = texture(textureSpecular1, vsIn.uv).rgb;
		spec *= dot(specVec, vec3(1.0));
	}

	if (hasNormal1) {
		vec3 normal = normalize(texture(textureNormal1, vsIn.uv).rgb * 2.0 - 1.0);
		gNormal = normalize(vsIn.tbn * normal);
	} else {
		gNormal = normalize(vsIn.tbn * vec3(0.0, 0.0, 1.0));
	}
	
	gAlbedoSpecular = vec4(texColor, spec);
} 
