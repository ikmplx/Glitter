layout (std140) uniform Vectors
{
	vec3 cameraPos;
};

layout (std140) uniform MatricesExt
{
	mat4 combinedNoDir;
	mat4 invCombined;
	mat4 invCombinedNoDir;
};

layout (std140) uniform Matrices
{
	mat4 proj;
	mat4 view;
	mat4 combined;
};
