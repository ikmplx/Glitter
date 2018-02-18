#pragma once

#include "Res.h"

namespace MyGL
{
	class Texture : public Resource
	{
	public:
		Texture(const std::string& name, const std::string& path);
		~Texture();
	
		void Bind();

	public:
		GLuint ID;
	};

	using TexturePtr = std::shared_ptr<Texture>;
}