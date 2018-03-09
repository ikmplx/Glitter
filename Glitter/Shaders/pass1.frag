#version 330 core

in VS_OUT {
	vec3 color;
	vec3 normal;
	vec3 pos;
	vec2 uv;
} vsIn;

out vec4 FragColor;

uniform bool hasDiffuse1;
uniform sampler2D textureDiffuse1;

uniform bool hasSpecular1;
uniform sampler2D textureSpecular1;

uniform float specularBase;

layout (location = 0) out vec4 gAlbedoSpecular;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gPosition;

void main()
{
	vec3 texColor = vsIn.color;
	float spec = specularBase;

	if (hasDiffuse1) {
		texColor *= texture(textureDiffuse1, vsIn.uv).rgb;
	}
	
	if (hasSpecular1) {
		vec3 specVec = texture(textureSpecular1, vsIn.uv).rgb;
		spec *= dot(specVec, vec3(1.0));
	}
	
	gAlbedoSpecular = vec4(texColor, spec);
	gNormal = vsIn.normal;
	gPosition = vsIn.pos;
} 
