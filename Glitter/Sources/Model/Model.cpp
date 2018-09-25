#include "stdafx.h"

#include "Model.h"
#include "Res/Res.h"
#include "Res/Texture.h"
#include "Mesh.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Material.h"
#include "Utils.h"

// System Headers
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace MyGL
{
	class ModelLoaderPrivate
	{
		friend class ModelLoader;

	public:
		ModelLoaderPrivate()
		{
		}

		EntityPtr Load(const std::string& path)
		{
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);// | aiProcess_FlipUVs);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
				std::cerr << "Error loading model " << path << ": " << importer.GetErrorString() << "\n";
				return std::make_shared<Entity>();
			}

			directory = path.substr(0, path.find_last_of('/'));
			return ProcessNode(scene->mRootNode, scene);
		}

	private:
		EntityPtr ProcessNode(aiNode* node, const aiScene* scene)
		{
			EntityPtr nodeEntity = std::make_shared<Entity>();

			for (unsigned iMesh = 0; iMesh < node->mNumMeshes; iMesh++) {
				aiMesh* mesh = scene->mMeshes[node->mMeshes[iMesh]];

				EntityPtr subMeshEntity = std::make_shared<Entity>();

				MeshPtr myMesh;
				MaterialPtr myMaterial;

				std::tie(myMesh, myMaterial) = ProcessMesh(mesh, scene);

				subMeshEntity->SetMesh(myMesh);
				subMeshEntity->SetMaterial(myMaterial);

				nodeEntity->AddChild(subMeshEntity);
			}

			for (unsigned iNode = 0; iNode < node->mNumChildren; iNode++) {
				aiNode* childNode = node->mChildren[iNode];
				EntityPtr subNodeEntity = ProcessNode(childNode, scene);

				nodeEntity->AddChild(subNodeEntity);
			}

			return nodeEntity;
		}
		std::tuple<MeshPtr, MaterialPtr> ProcessMesh(aiMesh* mesh, const aiScene* scene)
		{
			std::vector<Mesh::Vertex> vertices;
			std::vector<unsigned> indices;
			MaterialPtr myMaterial = std::make_shared<Material>();

			auto ProcessVec3 = [](const aiVector3D& vector) {
				return glm::vec3(vector.x, vector.y, vector.z);
			};

			auto ProcessColor = [](const aiColor4D& color) {
				return glm::vec3(color.r, color.g, color.b);
			};

			for (unsigned i = 0; i < mesh->mNumVertices; i++) {
				Mesh::Vertex vertex;
				vertex.pos = ProcessVec3(mesh->mVertices[i]);

				if (mesh->mNormals) {
					vertex.normal = ProcessVec3(mesh->mNormals[i]);
				}

				if (mesh->mTextureCoords[0]) {
					vertex.uv = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
				}

				if (mesh->mColors[0]) {
					vertex.color = ProcessColor(mesh->mColors[0][i]);
				}

				if (mesh->mTangents) {
					vertex.tangent = ProcessVec3(mesh->mTangents[i]);
					vertex.bitangent = ProcessVec3(mesh->mBitangents[i]);
				}

				vertices.push_back(vertex);
			}

			// Apply transform
			if (hasTransform) {
				glm::mat3 transformNormal = glm::mat3(glm::transpose(glm::inverse(transform)));
				for (auto& vertex : vertices) {
					vertex.pos = glm::vec3(transform * glm::vec4(vertex.pos, 1.f));

					if (mesh->mNormals) {
						vertex.normal = transformNormal * vertex.normal;
					}

					if (mesh->mTangents) {
						MyAssert("Not supported");
					}
				}
			}

			for (unsigned i = 0; i < mesh->mNumFaces; i++) {
				aiFace& face = mesh->mFaces[i];

				for (unsigned j = 0; j < face.mNumIndices; j++) {
					indices.push_back(face.mIndices[j]);
				}
			}

			if (mesh->mMaterialIndex >= 0) {
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

				LoadMaterialTextures(material, aiTextureType::aiTextureType_DIFFUSE, MyGL::TextureUsage::Diffuse, myMaterial);
				LoadMaterialTextures(material, aiTextureType::aiTextureType_SPECULAR, MyGL::TextureUsage::Specular, myMaterial);
				LoadMaterialTextures(material, aiTextureType::aiTextureType_NORMALS, MyGL::TextureUsage::Normal, myMaterial);
			}

			MeshPtr myMesh = std::make_shared<Mesh>(std::move(vertices), std::move(indices));

			return std::make_tuple(myMesh, myMaterial);
		}
		void LoadMaterialTextures(aiMaterial* material, aiTextureType textureType, TextureUsage textureUsage, MaterialPtr outMaterial) {
			unsigned nTextures = material->GetTextureCount(textureType);
			aiString path;

			for (unsigned i = 0; i < nTextures; i++) {
				material->GetTexture(textureType, i, &path);

				std::string fullPath = directory + '/' + path.C_Str();

				TexturePtr texture = ResourceManager::Instance()->EnsureTexture(std::string("m:") + fullPath, fullPath);
				texture->Usage = textureUsage;

				outMaterial->SetTexture(texture, textureUsage);
			}
		}

	private:
		std::vector<MeshPtr> meshes;
		std::string directory;
		EntityPtr entity;

		glm::mat4 transform = glm::mat4(1.f);
		bool hasTransform = false;
	};


	ModelLoader::ModelLoader()
		: d(new ModelLoaderPrivate())
	{
	}

	ModelLoader::~ModelLoader() = default;

	EntityPtr ModelLoader::Load(const std::string& path)
	{
		return d->Load(path);
	}

	EntityPtr ModelLoader::LoadModel(const std::string & path)
	{
		ModelLoader l;
		return l.Load(path);
	}

	void ModelLoader::SetTransform(const glm::mat4 & transform)
	{
		d->transform = transform;
		d->hasTransform = true;
	}
}
