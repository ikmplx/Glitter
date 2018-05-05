#pragma once

#include "stdafx.h"

namespace MyGL
{
	class Attachment;
	using AttachmentPtr = std::shared_ptr<Attachment>;

	class Framebuffer;
	using FramebufferPtr = std::shared_ptr<Framebuffer>;

	class DeferredRenderer;
	using DeferredRendererPtr = std::shared_ptr<DeferredRenderer>;

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

	class RigidBody;
	using RigidBodyPtr = std::shared_ptr<RigidBody>;
	using RigidBodyWeakPtr = std::weak_ptr<RigidBody>;

	class Camera;

	enum class TextureUsage;
}