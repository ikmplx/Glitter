#version 330 core

in vec3 vColor;
in vec2 vPos;
in vec2 vUv;

out vec4 FragColor;

uniform sampler2D tex;

void main()
{
	vec4 texColor = texture(tex, vUv);
	FragColor = texColor;

    FragColor = vec4(vColor.rgb * texColor.rgb, texColor.a);
} 