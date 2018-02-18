#pragma once

namespace MyGL
{
	class Mesh
	{
	public:
		struct Vertex
		{
			glm::vec3 pos;
			glm::vec3 normal;
			glm::vec2 uv;
			glm::vec3 color;

			glm::vec3 tangent;
			glm::vec3 bitangent;
		};

		struct Texture
		{
			GLuint id;
			std::string type;
			std::string path;
		};


		std::vector<Vertex> vertices;
		std::vector<unsigned> indices;

		std::vector<Texture> textures;

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, std::vector<Texture> textures);

	private:
		GLuint _vba;
		GLuint _vbo;
		GLuint _ibo;
	};
}