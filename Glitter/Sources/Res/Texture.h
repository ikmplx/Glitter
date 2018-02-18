#pragma once

#include "Res.h"

namespace MyGL
{
	enum class TextureUsage
	{
		Diffuse,
		Specular,
		Normal,
		None
	};

	class Texture : public Resource
	{
	public:
		Texture(const std::string& name, const std::string& path);
		~Texture();
	
		void Bind();

	public:
		GLuint ID = 0;
		TextureUsage Usage = TextureUsage::None;
	};

	using TexturePtr = std::shared_ptr<Texture>;
}