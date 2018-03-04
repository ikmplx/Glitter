#version 330 core

out vec2 vUv;

void main()
{
	vec2 pos = vec2(gl_VertexID & 1, (gl_VertexID & 2) >> 1);
	
	gl_Position = vec4(pos * 2.0 - 1.0, 1.0, 1.0);
	vUv = pos;
}
