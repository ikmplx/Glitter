#include "stdafx.h"

#include <functional>


#include "Mesh.h"

#include "Res/Texture.h"
#include "Res/Shader.h"
#include "Material.h"
#include "Utils.h"

namespace MyGL
{
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned> indices)
		: vertices(vertices)
		, indices(indices)
	{
		glGenVertexArrays(1, &_vao);
		glGenBuffers(1, &_vbo);
		glGenBuffers(1, &_ibo);

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

	void Mesh::FillBulletMeshInterface(btTriangleIndexVertexArray& btArray)
	{
		auto btMesh = std::make_unique<btIndexedMesh>();
		btMesh->m_numTriangles = int(indices.size() / 3);
		btMesh->m_triangleIndexBase = reinterpret_cast<const unsigned char *>(indices.data());
		btMesh->m_triangleIndexStride = 3 * sizeof(int);
		btMesh->m_numVertices = int(vertices.size());
		btMesh->m_vertexBase = reinterpret_cast<const unsigned char *>(vertices.data());
		btMesh->m_vertexStride = sizeof(Vertex);
		
		btArray.addIndexedMesh(*btMesh);
	}

	void Mesh::Draw(ShaderPtr shader)
	{
		shader->Bind();

		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, (GLsizei) indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void Mesh::Draw()
	{
		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, (GLsizei) indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	GLuint Mesh::GetVAO()
	{
		return _vao;
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

		return std::make_shared<Mesh>(std::move(vertices), std::move(indices));
	}


	MeshPtr Primitives::CreatePlane(float width, float height, float textureScale)
	{
		std::vector<Mesh::Vertex> vertices;

		auto AddVertex = [&vertices](float x, float y, float z, float u, float v) {
			vertices.push_back({ glm::vec3(x, y, z), glm::vec3(0, 1, 0), glm::vec2(u, v), glm::vec3(1.0), glm::vec3(0, 0, -1), glm::vec3(1, 0, 0) });
		};

		float halfWidth = width * 0.5f;
		float halfHeight = height * 0.5f;

		AddVertex(-halfWidth, 0.f, -halfHeight, 0.f, 0.f);
		AddVertex(-halfWidth, 0.f, halfHeight, 0.f, height * textureScale);
		AddVertex(halfWidth,  0.f,  halfHeight, width * textureScale, height * textureScale);
		AddVertex(halfWidth,  0.f, -halfHeight, width * textureScale, 0.f);

		std::vector<unsigned> indices{
			0, 1, 2,
			0, 2, 3
		};

		return std::make_shared<Mesh>(std::move(vertices), std::move(indices));
	}

	MeshPtr Primitives::CreateSphere(float radius, int subdivisions)
	{
		struct Triangle
		{
			Triangle(GLuint a, GLuint b, GLuint c)
			{
				index[0] = a;
				index[1] = b;
				index[2] = c;
			}

			GLuint index[3];
		};

		using Indices = std::vector<Triangle>;
		using Vertices = std::vector<glm::vec3>;

		Vertices vertices;
		Indices indices;

		const float X = 0.525731112119133606f;
		const float Z = 0.850650808352039932f;
		const float N = 0.f;

		vertices = {
			{-X, N, Z}, {X, N, Z}, {-X, N, -Z}, {X, N, -Z},
			{N, Z, X}, {N, Z, -X}, {N, -Z, X}, {N, -Z, -X},
			{Z, X, N}, {-Z, X, N}, {Z, -X, N}, {-Z, -X, N}
		};

		indices = {
			{0, 4, 1}, {0, 9, 4}, {9, 5, 4}, {4, 5, 8}, {4, 8, 1},
			{8, 10, 1}, {8, 3, 10}, {5, 3, 8}, {5, 2, 3}, {2, 7, 3},
			{7, 10, 3}, {7, 6, 10}, {7, 11, 6}, {11, 0, 6}, {0, 1, 6},
			{6, 1, 10}, {9, 0, 11}, {9, 11, 2}, {9, 2, 5}, {7, 2, 11}
		};

		struct IntPairHasher
		{
			size_t operator()(const std::pair<GLuint, GLuint>& p) const
			{
				return Utils::HashCombine(p.first, p.second);
			}
		};

		using Lookup = std::unordered_map<std::pair<GLuint, GLuint>, GLuint, IntPairHasher>;

		auto FixWinding = [](Indices& indices, const Vertices& vertices) {
			for (auto& triangle : indices) {
				const glm::vec3 v1 = vertices[triangle.index[0]];
				const glm::vec3 v2 = vertices[triangle.index[1]];
				const glm::vec3 v3 = vertices[triangle.index[2]];

				glm::vec3 normal = glm::cross(v2 - v1, v3 - v1);
				if (glm::dot(normal, v1) < 0) {
					std::swap(triangle.index[0], triangle.index[1]);
				}
			}
		};

		auto SubdivideEdge = [](GLuint i1, GLuint i2, Vertices& vertices, Lookup& lookup) -> GLuint {
			if (i1 > i2) {
				std::swap(i1, i2);
			}

			auto needle = std::make_pair(i1, i2);
			auto found = lookup.find(needle);
			if (found == lookup.end()) {
				const glm::vec3& v1 = vertices[i1];
				const glm::vec3& v2 = vertices[i2];
				glm::vec3 mid = glm::normalize(v1 + v2);

				GLuint index = (GLuint) vertices.size();
				vertices.push_back(mid);

				lookup[needle] = index;
				return index;
			}
			else {
				return found->second;
			}
		};

		auto Subdivide = [&SubdivideEdge](const Indices& indices, Vertices& vertices) -> Indices {
			Indices result;
			Lookup lookup;

			for (const auto& triangle : indices) {
				GLuint mid[3];

				for (int i = 0; i < 3; i++) {
					int a = i;
					int b = (i + 1) % 3;

					GLuint i1 = triangle.index[a];
					GLuint i2 = triangle.index[b];

					mid[i] = SubdivideEdge(i1, i2, vertices, lookup);
				}

				result.emplace_back(triangle.index[0], mid[0], mid[2]);
				result.emplace_back(triangle.index[1], mid[1], mid[0]);
				result.emplace_back(triangle.index[2], mid[2], mid[1]);
				result.emplace_back(mid[0], mid[1], mid[2]);
			}

			return result;
		};

		FixWinding(indices, vertices);

		for (int i = 0; i < subdivisions; i++) {
			indices = Subdivide(indices, vertices);
		}

		std::vector<Mesh::Vertex> meshVertices;
		std::vector<unsigned> meshIndices;

		meshVertices.reserve(vertices.size());
		meshIndices.reserve(indices.size() * 3);

		for (auto& vert : vertices) {
			// TODO: color используется для передачи uvw координат во фрагментный шейдер
			// это уродливо, но позволяет использовать стандартный вершинный шейдер
			meshVertices.push_back(Mesh::Vertex {vert * radius, vert, glm::vec2(0.f, 0.f), vert, glm::vec3(0.f), glm::vec3(0.f)});
		}

		for (size_t i = 0; i < indices.size(); i++) {
			meshIndices.push_back(indices[i].index[0]);
			meshIndices.push_back(indices[i].index[1]);
			meshIndices.push_back(indices[i].index[2]);
		}

		return std::make_shared<Mesh>(std::move(meshVertices), std::move(meshIndices));
	}
}

