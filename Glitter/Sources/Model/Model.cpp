#include "stdafx.h"

#include "Model.h"
#include "Res/Res.h"
#include "Res/Texture.h"
#include "Mesh.h"
#include "Scene/Scene.h"

// System Headers
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace MyGL
{
	struct ModelLoaderHelper
	{
		std::vector<MeshPtr> meshes;
		std::string directory;
		EntityPtr entity;

		EntityPtr ProcessNode(aiNode* node, const aiScene* scene)
		{
			EntityPtr nodeEntity = std::make_shared<Entity>();

			for (unsigned iMesh = 0; iMesh < node->mNumMeshes; iMesh++) {
				aiMesh* mesh = scene->mMeshes[node->mMeshes[iMesh]];

				EntityPtr subMeshEntity = std::make_shared<Entity>();
				subMeshEntity->SetMesh(ProcessMesh(mesh, scene));

				nodeEntity->AddChild(subMeshEntity);
			}

			for (unsigned iNode = 0; iNode < node->mNumChildren; iNode++) {
				aiNode* childNode = node->mChildren[iNode];
				EntityPtr subNodeEntity = ProcessNode(childNode, scene);

				nodeEntity->AddChild(subNodeEntity);
			}

			return nodeEntity;
		}

		MeshPtr ProcessMesh(aiMesh* mesh, const aiScene* scene)
		{
			std::vector<Mesh::Vertex> vertices;
			std::vector<unsigned> indices;
			std::vector<TexturePtr> textures;

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

			for (unsigned i = 0; i < mesh->mNumFaces; i++) {
				aiFace& face = mesh->mFaces[i];

				for (unsigned j = 0; j < face.mNumIndices; j++) {
					indices.push_back(face.mIndices[j]);
				}
			}

			if (mesh->mMaterialIndex >= 0) {
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

				LoadMaterialTextures(material, aiTextureType::aiTextureType_DIFFUSE, MyGL::TextureUsage::Diffuse, textures);
				LoadMaterialTextures(material, aiTextureType::aiTextureType_SPECULAR, MyGL::TextureUsage::Specular, textures);
				LoadMaterialTextures(material, aiTextureType::aiTextureType_NORMALS, MyGL::TextureUsage::Normal, textures);
			}

			return std::make_shared<Mesh>(std::move(vertices), std::move(indices), std::move(textures));
		}

		void LoadMaterialTextures(aiMaterial* material, aiTextureType textureType, TextureUsage textureUsage, std::vector<TexturePtr>& out)
		{
			unsigned nTextures = material->GetTextureCount(textureType);
			aiString path;

			for (unsigned i = 0; i < nTextures; i++) {
				material->GetTexture(textureType, i, &path);

				std::string fullPath = directory + '/' + path.C_Str();

				TexturePtr texture = ResourceManager::Instance()->EnsureTexture(std::string("m:") + fullPath, fullPath);
				texture->Usage = textureUsage;

				out.push_back(texture);
			}
		}
	};


	EntityPtr ModelLoader::LoadModel(const std::string & path)
	{
		ModelLoaderHelper d;

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);// | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cerr << "Error loading model " << path << ": " << importer.GetErrorString() << "\n";
			return std::make_shared<Entity>();
		}

		d.directory = path.substr(0, path.find_last_of('/'));
		return d.ProcessNode(scene->mRootNode, scene);
	}
}

