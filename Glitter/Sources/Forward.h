#pragma once

#include "stdafx.h"

namespace MyGL
{
	class Shader;
	using ShaderPtr = std::shared_ptr<Shader>;

	class Texture;
	using TexturePtr = std::shared_ptr<Texture>;

	class Cubemap;
	using CubemapPtr = std::shared_ptr<Cubemap>;

	class Resource;
	using ResourcePtr = std::shared_ptr<Resource>;

	class Mesh;
	using MeshPtr = std::shared_ptr<Mesh>;

	class Scene;
	using ScenePtr = std::shared_ptr<Scene>;

	class Entity;
	using EntityPtr = std::shared_ptr<Entity>;
	using EntityWeakPtr = std::weak_ptr<Entity>;

	struct Component;
	using ComponentPtr = std::shared_ptr<Component>;
	using ComponentWeakPtr = std::weak_ptr<Component>;

	class Material;
	using MaterialPtr = std::shared_ptr<Material>;

	enum class TextureUsage;
}