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
			glUniform1i(shader->GetUniformLocation("texture_diffuse1"), iSampler);
			iSampler++;
		}

		if (normal) {
			glActiveTexture(GL_TEXTURE0 + iSampler);
			normal->Bind();
			glUniform1i(shader->GetUniformLocation("texture_normal1"), iSampler);
			iSampler++;
		}

		if (specular) {
			glActiveTexture(GL_TEXTURE0 + iSampler);
			specular->Bind();
			glUniform1i(shader->GetUniformLocation("specular_specular1"), iSampler);
			iSampler++;
		}

		glActiveTexture(GL_TEXTURE0);
	}
}
