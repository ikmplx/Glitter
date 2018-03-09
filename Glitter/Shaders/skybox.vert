#version 330 core

layout (std140) uniform MatricesExt
{
	mat4 combinedNoDir;
	mat4 invCombined;
	mat4 invCombinedNoDir;
};

out vec3 vUvw;

void main()
{
	vec2 pos = vec2(gl_VertexID & 1, (gl_VertexID & 2) >> 1);
	pos = pos * 2.0 - 1.0;

	vec4 dir = invCombinedNoDir * vec4(pos, 1.0, 1.0);

	gl_Position = vec4(pos, 1.0, 1.0);
	vUvw = dir.xyz / dir.w;
	vUvw.z = -vUvw.z;
}
