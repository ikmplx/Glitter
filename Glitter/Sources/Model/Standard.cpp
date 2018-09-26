// ¯\_(ツ)_/¯

#include "stdafx.h"

#include "Material.h"
#include "Res/Texture.h"
#include "Res/Shader.h"

namespace MyGL
{
	Material::Material(TexturePtr diffuse)
		: diffuse(diffuse)
	{
	}
	Material::Material() = default;

	Material::~Material() = default;

	void Material::SetTexture(TexturePtr texture, TextureUsage textureUsage)
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
		}
	}

	void Material::Bind(const ShaderPtr & shader)
	{
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
}
