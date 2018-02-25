#include "stdafx.h"

#include "Texture.h"

#include "chrono"

#include <stb_image.h>



namespace MyGL
{
	Texture::Texture(const std::string& name, const std::string& path)
		: Resource(name, ResourceType::Texture)
	{
		auto timeBefore = std::chrono::steady_clock::now();

		GLuint texture;

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.f);

		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		auto* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);

			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cerr << "Can't load texture " << path << "\n";
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		ID = texture;

		std::cout << "[ResourceManager] Texture "
			<< GetName()
			<< " loaded in "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - timeBefore).count()
			<< "\n";
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &ID);
	}

	void Texture::Bind()
	{
		if (ID) {
			glBindTexture(GL_TEXTURE_2D, ID);
		}
	}



	Cubemap::Cubemap(const std::string& name, const char* prefix, const char* extension, std::vector<const char*> suffixes)
		: Resource(name, ResourceType::Cubemap)
	{
		if (suffixes.size() < 6) {
			return;
		}

		GLuint texture;

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, channels;
		stbi_set_flip_vertically_on_load(false);
		char path[1024];

		for (int i = 0; i < 6; i++) {
			snprintf(path, sizeof(path), "%s%s%s", prefix, suffixes[i], extension);

			auto* data = stbi_load(path, &width, &height, &channels, 0);
			if (data) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else {
				std::cerr << "Can't load cubemap face " << path << "\n";
			}
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		ID = texture;
	}

	Cubemap::~Cubemap()
	{
		glDeleteTextures(1, &ID);
	}

	void Cubemap::Bind()
	{
		if (ID) {
			glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
		}
	}
}