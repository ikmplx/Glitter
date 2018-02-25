#version 330 core

layout (location = 0) in vec3 aPos;

layout (std140) uniform Matrices
{
    mat4 proj;
    mat4 view;
	mat4 combined;
};

out vec3 vUvw;

void main()
{
	mat4 combinedNoTranslation = proj * mat4(mat3(view));
	vec4 pos = combinedNoTranslation * vec4(aPos, 1.0);
	gl_Position = pos.xyww;
	vUvw = aPos;
}
