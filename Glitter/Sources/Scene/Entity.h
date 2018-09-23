// ¯\_(ツ)_/¯

#pragma once

#include "Forward.h"

namespace MyGL
{
	class Entity : public std::enable_shared_from_this<Entity>
	{
		friend class RigidBody;
		friend class Scene;
		friend class System;

	public:
		Entity();
		~Entity();

		template <typename Fun>
		void Traverse(Fun fun);

		const glm::mat4& GetGlobalTransform() const;
		glm::vec3 GetGlobalPosition() const;

		EntityPtr Clone();

		EntityPtr CreateChild();// Can't be called on entities, attached to scene
		void AddChild(EntityPtr child);// Can't be called on entities, attached to scene
	
		void SetMesh(MeshPtr mesh);
		MeshPtr GetMesh();

		void SetMaterial(MaterialPtr material);
		MaterialPtr GetMaterial();

		void SetRigidBody(RigidBodyPtr rigidBody);

		void Draw(ShaderPtr shader);

		void InvalidateTransform();

	private:
		ComponentPtr FindComponent(const std::type_info& typeInfo);

	public:
		glm::quat rotation = glm::angleAxis(0.f, glm::vec3(0, 0, 1));
		glm::vec3 position = glm::vec3(0);
		glm::vec3 scale = glm::vec3(1);

	private:
		EntityWeakPtr _parent;
		std::vector<EntityPtr> _children;

		MeshPtr _mesh;
		MaterialPtr _material;

		std::vector<ComponentPtr> _components;

		ComponentTypeSet _componentTypeSet;
		SystemTypeSet _systemTypeSet;

		mutable glm::mat4 _globalTransform = glm::mat4(1.f);
		mutable bool _isGlobalTransformNeedUpdate = true;

		RigidBodyPtr _rigidBody;
	};

	template<typename Fun>
	inline void Entity::Traverse(Fun fun)
	{
		fun(shared_from_this());
		for (auto& child : _children) {
			child->Traverse(fun);
		}
	}
}