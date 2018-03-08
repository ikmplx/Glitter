#version 330 core

in VS_OUT {
	vec2 uv;
	vec3 color;
	vec3 pos;
} vsIn;

out vec4 FragColor;

uniform bool has_diffuse1;
uniform sampler2D texture_diffuse1;

void main()
{
	vec3 texColor = vsIn.color;

	if (has_diffuse1) {
		texColor *= texture(texture_diffuse1, vsIn.uv).rgb;
	}
	
	FragColor = vec4(texColor, 1.0);
	// FragColor = vec4(vsIn.color, 1.0);
} 