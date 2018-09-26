// ¯\_(ツ)_/¯

#pragma once

#include "Forward.h"

namespace MyGL
{
	class Material
	{
	public:
		Material();
		virtual ~Material();
		virtual void Bind(const glm::mat4& modelTransform) = 0;

	};
	using MaterialPtr = std::shared_ptr<Material>;

	class StandardMaterial : public Material
	{
	public:
		StandardMaterial();
		StandardMaterial(TexturePtr diffuse);
		~StandardMaterial();

		void Bind(const glm::mat4& modelTransform) override;

		void SetTexture(TexturePtr texture, TextureUsage textureUsage);

		TexturePtr diffuse;
		TexturePtr specular;
		TexturePtr normal;
		ShaderPtr shader;

		float specularBase = 1.0;
	};

	class VertexColorMaterial : public Material
	{
	public:
		VertexColorMaterial();
		~VertexColorMaterial();

		void Bind(const glm::mat4& modelTransform) override;

		ShaderPtr shader;
		float specularBase = 1.0;
	};
}