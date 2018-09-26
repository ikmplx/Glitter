// ¯\_(ツ)_/¯

#pragma once

#include "Forward.h"

namespace MyGL
{
	class Material
	{
	public:
		Material(TexturePtr diffuse);

		Material();
		~Material();

		void SetTexture(TexturePtr texture, TextureUsage textureUsage);
		void Bind(const ShaderPtr& shader);

		TexturePtr diffuse;
		TexturePtr specular;
		TexturePtr normal;
		float specularBase = 1.0;
	};
	using MaterialPtr = std::shared_ptr<Material>;
}