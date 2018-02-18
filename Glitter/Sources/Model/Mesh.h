#pragma once

#include "Forward.h"

namespace MyGL
{
	class Mesh
	{
	public:
		struct Vertex
		{
			glm::vec3 pos = glm::vec3(0.f);
			glm::vec3 normal = glm::vec3(0.f);
			glm::vec2 uv = glm::vec2(0.f);
			glm::vec3 color = glm::vec3(0.f);

			glm::vec3 tangent = glm::vec3(0.f);
			glm::vec3 bitangent = glm::vec3(0.f);
		};

		std::vector<Vertex> vertices;
		std::vector<unsigned> indices;

		std::vector<TexturePtr> textures;

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, std::vector<TexturePtr> textures);
		~Mesh();

		void Draw(ShaderPtr shader);

	private:
		GLuint _vao;
		GLuint _vbo;
		GLuint _ibo;
	};
	using MeshPtr = std::shared_ptr<Mesh>;

	namespace Primitives {
		MeshPtr CreateCube();
	}
}