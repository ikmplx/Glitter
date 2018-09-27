#version 330 core

out vec2 vUv;

uniform float quadScale;
uniform vec2 quadShift;

void main()
{
	vec2 pos = vec2(gl_VertexID & 1, (gl_VertexID & 2) >> 1);
	
	// 00, 01, 10, 11 =>
	// 00, 10, 01, 11

	gl_Position = vec4(pos * quadScale + quadShift, 1.0, 1.0);
	vUv = pos;
}
