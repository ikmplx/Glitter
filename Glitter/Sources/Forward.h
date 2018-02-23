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

	class Scene;
	using ScenePtr = std::shared_ptr<Scene>;

	class Entity;
	using EntityPtr = std::shared_ptr<Entity>;
	using EntityWeakPtr = std::weak_ptr<Entity>;
}