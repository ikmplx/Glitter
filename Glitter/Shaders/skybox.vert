#version 330 core

@include MatricesExtUbo

out vec3 vUvw;
out vec3 vDir;

uniform mat4 skyTransform;

void main()
{
	vec2 pos = vec2(gl_VertexID & 1, (gl_VertexID & 2) >> 1);
	pos = pos * 2.0 - 1.0;
	gl_Position = vec4(pos, 1.0, 1.0);

	vec4 dir = invCombinedNoDir * vec4(pos, 1.0, 1.0);
	dir.xyz /= dir.w;

	vDir = vec3(dir);
	vUvw = vec3(skyTransform * dir);
}
