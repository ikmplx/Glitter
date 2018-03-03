#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out vec2 vUv;

void main() {    
	gl_Position = vec4(-1.0, -1.0, 0.0, 1.0); 
	vUv = vec2(0.0, 0.0);
	EmitVertex();

	gl_Position = vec4(-1.0, 1.0, 0.0, 1.0);
	vUv = vec2(0.0, 1.0);
	EmitVertex();
	
	gl_Position = vec4(1.0, -1.0, 0.0, 1.0);
	vUv = vec2(1.0, 0.0);
	EmitVertex();

	gl_Position = vec4(1.0, 1.0, 0.0, 1.0);
	vUv = vec2(1.0, 1.0);
	EmitVertex();
	
	EndPrimitive();
}
