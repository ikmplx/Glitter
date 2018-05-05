// ¯\_(ツ)_/¯

#pragma once

#include "Forward.h"

namespace MyGL
{
	class Entity : public std::enable_shared_from_this<Entity>
	{
		friend class Scene;

	public:
		Entity();
		~Entity();

		template <typename Fun>
		void Traverse(Fun fun);

		const glm::mat4& GetGlobalTransform() const;
		EntityPtr Clone();

		EntityPtr CreateChild();
		void AddChild(EntityPtr child);

		template <typename T>
		void AddComponent();

		template <typename T>
		std::shared_ptr<T> FindComponent();

		void SetMesh(MeshPtr mesh);
		MeshPtr GetMesh();

		void Draw(ShaderPtr shader);

		void InvalidateTransform();

	private:
		void AddComponent(ComponentPtr component);
		ComponentPtr FindComponent(const std::type_info& typeInfo);

		void UpdateGlobalTransform() const;

	public:
		glm::quat rotation;
		glm::vec3 position;
		glm::vec3 scale;

	private:
		EntityWeakPtr _parent;
		std::vector<EntityPtr> _children;

		MeshPtr _mesh;

		std::vector<ComponentPtr> _components;

		mutable glm::mat4 _globalTransform;
		mutable bool _isGlobalTransformNeedUpdate = true;
	};

	template<typename Fun>
	inline void Entity::Traverse(Fun fun)
	{
		fun(*this);
		for (auto& child : _children) {
			child->Traverse(fun);
		}
	}

	template<typename T>
	inline void Entity::AddComponent()
	{
		ComponentPtr component = std::static_pointer_cast<Component>(std::make_shared<T>());
		AddComponent(component);
	}

	template<typename T>
	inline std::shared_ptr<T> Entity::FindComponent()
	{
		return std::static_pointer_cast<T>(FindComponent(typeid(T)));
	}
}