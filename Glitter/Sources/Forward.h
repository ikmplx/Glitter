#pragma once

#include "stdafx.h"

namespace MyGL
{
	class Shader;
	using ShaderPtr = std::shared_ptr<Shader>;

	class Texture;
	using TexturePtr = std::shared_ptr<Texture>;

	class Resource;
	using ResourcePtr = std::shared_ptr<Resource>;

	class Mesh;
	using MeshPtr = std::shared_ptr<Mesh>;
}