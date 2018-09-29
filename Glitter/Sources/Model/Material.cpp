// ¯\_(ツ)_/¯

#include "stdafx.h"

#include "Material.h"
#include "Res/Texture.h"
#include "Res/Shader.h"
#include "Utils.h"

namespace MyGL
{
	Material::Material() = default;
	Material::~Material() = default;

	StandardMaterial::StandardMaterial()
		: StandardMaterial(nullptr)
	{
	}

	StandardMaterial::StandardMaterial(TexturePtr diffuse)
		: diffuse(diffuse)
		, shader(ResourceManager::Instance()->GetShader("SurfaceStandard"))
	{
	}

	StandardMaterial::~StandardMaterial() = default;

	void StandardMaterial::Bind(const glm::mat4& modelTransform)
	{
		shader->Bind();
		shader->SetMatrix("model", modelTransform);
		int iSampler = 0;
		if (diffuse) {
			glActiveTexture(GL_TEXTURE0 + iSampler);
			diffuse->Bind();
			glUniform1i(shader->GetUniformLocation("textureDiffuse1"), iSampler);
			glUniform1i(shader->GetUniformLocation("hasDiffuse1"), true);
			iSampler++;
		}
		else {
			glUniform1i(shader->GetUniformLocation("hasDiffuse1"), false);
		}

		if (normal) {
			glActiveTexture(GL_TEXTURE0 + iSampler);
			normal->Bind();
			glUniform1i(shader->GetUniformLocation("textureNormal1"), iSampler);
			glUniform1i(shader->GetUniformLocation("hasNormal1"), true);
			iSampler++;
		}
		else {
			glUniform1i(shader->GetUniformLocation("hasNormal1"), false);
		}

		if (specular) {
			glActiveTexture(GL_TEXTURE0 + iSampler);
			specular->Bind();
			glUniform1i(shader->GetUniformLocation("specularSpecular1"), iSampler);
			glUniform1i(shader->GetUniformLocation("hasSpecular1"), true);
			iSampler++;
		}
		else {
			glUniform1i(shader->GetUniformLocation("hasSpecular1"), false);
		}

		shader->SetFloat("specularBase", specularBase);

		glActiveTexture(GL_TEXTURE0);
	}

	void StandardMaterial::SetTexture(TexturePtr texture, TextureUsage textureUsage)
	{
		switch (textureUsage) {
		case TextureUsage::Diffuse:
			diffuse = texture;
			break;

		case TextureUsage::Normal:
			normal = texture;
			break;

		case TextureUsage::Specular:
			specular = texture;
			break;

		default:
			MyAssert(false);
			break;
		}
	}

	VertexColorMaterial::VertexColorMaterial()
		: shader(ResourceManager::Instance()->GetShader("SurfaceVertexColor"))
	{
	}

	VertexColorMaterial::~VertexColorMaterial() = default;

	void VertexColorMaterial::Bind(const glm::mat4& modelTransform)
	{
		shader->Bind();
		shader->SetMatrix("model", modelTransform);
	}


	CubemapMaterial::CubemapMaterial(const CubemapPtr& cubemap)
		: shader(ResourceManager::Instance()->GetShader("SurfaceCubemap"))
		, cubemap(cubemap)
	{
	}

	CubemapMaterial::~CubemapMaterial() = default;

	void CubemapMaterial::Bind(const glm::mat4& modelTransform)
	{
		shader->Bind();
		shader->SetMatrix("model", modelTransform);
		glActiveTexture(GL_TEXTURE0);
		cubemap->Bind();
		shader->SetInt("texture1", 0);
	}
}
