#include "stdafx.h"

#include "Mesh.h"

#include "Res/Texture.h"
#include "Res/Shader.h"

namespace MyGL
{
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, std::vector<TexturePtr> textures)
		: vertices(vertices)
		, indices(indices)
		, textures(textures)
	{
		glCreateVertexArrays(1, &_vao);
		glCreateBuffers(1, &_vbo);
		glCreateBuffers(1, &_ibo);

		glBindVertexArray(_vao);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

		// pos
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, pos));

		// normal
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		// uv
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		// color
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		// tangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

		// bitangent
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

		glBindVertexArray(0);

	}

	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &_vao);

		glDeleteBuffers(1, &_vbo);
		glDeleteBuffers(1, &_ibo);
	}

	void Mesh::Draw(ShaderPtr shader)
	{
		static char uniformName[255];

		unsigned specularN = 0;
		unsigned diffuseN = 0;
		unsigned normalN = 0;
		unsigned unknownN = 0;

		unsigned samplerN = 0;
		
		shader->Bind();

		for (const auto& texture : textures) {
			glActiveTexture(GL_TEXTURE0 + samplerN);
			texture->Bind();

			const char* usageName = "unknown";
			unsigned usageN = 0;
			if (texture->Usage == TextureUsage::Diffuse || texture->Usage == TextureUsage::None) {
				usageName = "diffuse";
				usageN = ++diffuseN;
			}
			else if (texture->Usage == TextureUsage::Specular) {
				usageName = "specular";
				usageN = ++specularN;
			}
			else if (texture->Usage == TextureUsage::Normal) {
				usageName = "normal";
				usageN = ++normalN;
			}
			else {
				usageN = ++unknownN;
			}

			snprintf(uniformName, sizeof(uniformName), "texture_%s%d", usageName, usageN);
			glUniform1i(shader->GetUniformLocation(uniformName), samplerN);

			samplerN++;
		}

		glActiveTexture(GL_TEXTURE0);

		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}


	MeshPtr Primitives::CreateCube()
	{
		std::vector<Mesh::Vertex> vertices;

		auto AddVertex = [&vertices](float x, float y, float z, float u, float v, float nx, float ny, float nz) {
			vertices.push_back({ glm::vec3(x, y, z), glm::vec3(nx, ny, nz), glm::vec2(u, v), glm::vec3(1.0), glm::vec3(), glm::vec3() });
		};

		AddVertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
		AddVertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f);
		AddVertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f);
		AddVertex(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f);

		AddVertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		AddVertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		AddVertex(0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		AddVertex(-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);

		AddVertex(-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f);
		AddVertex(-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f);
		AddVertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f);
		AddVertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);

		AddVertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
		AddVertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
		AddVertex(0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f);
		AddVertex(0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);

		AddVertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f);
		AddVertex(0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f);
		AddVertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f);
		AddVertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f);

		AddVertex(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
		AddVertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f);
		AddVertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		AddVertex(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

		std::vector<unsigned> indices {
			0, 2, 1,
			0, 3, 2,

			4, 5, 6,
			4, 6, 7,

			8, 9, 10,
			8, 10, 11,

			12, 14, 13,
			12, 15, 14,

			16, 17, 18,
			16, 18, 19,

			20, 22, 21,
			20, 23, 22
		};

		std::vector<TexturePtr> textures{ ResourceManager::Instance()->GetTexture("Crate") };

		return std::make_shared<Mesh>(std::move(vertices), std::move(indices), std::move(textures));
	}
}

