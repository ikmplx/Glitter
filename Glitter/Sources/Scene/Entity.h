// ¯\_(ツ)_/¯

#pragma once

#include "Forward.h"

namespace MyGL
{
	class PhysicsMotionState;

	class Entity : public std::enable_shared_from_this<Entity>
	{
		friend class PhysicsMotionState;
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

		void SetId(int id);
		int GetId() const;

		void SetName(std::string name);
		std::string GetName() const;

		void Draw();

		void InvalidateTransform();

		ComponentPtr FindComponent(const std::type_info& typeInfo);

		template <typename T>
		std::shared_ptr<T> FindComponent()
		{
			return std::static_pointer_cast<T>(FindComponent(typeid(T)));
		}

	public:
		glm::quat rotation = glm::angleAxis(0.f, glm::vec3(0, 0, 1));
		glm::vec3 position = glm::vec3(0);
		glm::vec3 scale = glm::vec3(1);

		// TODO: can convert bullet's btTransform to rotation and position above and get rid of externalTransform
		bool hasExternalTransform = false;
		glm::mat4 externalTransform = glm::mat4(1.f);

	private:
		int _id = -1;
		std::string _name;

		EntityWeakPtr _parent;
		std::vector<EntityPtr> _children;

		MeshPtr _mesh;
		MaterialPtr _material;

		std::vector<ComponentPtr> _components;

		ComponentTypeSet _componentTypeSet;
		SystemTypeSet _systemTypeSet;

		mutable glm::mat4 _globalTransform = glm::mat4(1.f);
		mutable bool _isGlobalTransformNeedUpdate = true;
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