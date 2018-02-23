#pragma once

#include "Forward.h"

namespace MyGL
{
	class Entity : public std::enable_shared_from_this<Entity>
	{
	public:
		Entity();
		~Entity();

		template <typename Fun>
		void Traverse(Fun fun);

		void CalcGlobalTransform();
		EntityPtr Clone();

		EntityPtr CreateChild();
		void AddChild(EntityPtr child);

		void SetMesh(MeshPtr mesh);
		
		void Draw(ShaderPtr shader);

	public:
		glm::quat rotation;
		glm::vec3 position;
		glm::vec3 scale;

	private:
		MeshPtr _mesh;

		std::vector<EntityPtr> _children;
		EntityWeakPtr _parent;

		glm::mat4 _globalTransform;

		glm::quat _globalRotation;
		glm::vec3 _globalPosition;
		glm::vec3 _globalScale;
	};

	class Scene
	{
	public:
		Scene();
		~Scene();

		EntityPtr CreateEntity();

		void AddEntity(EntityPtr ptr);
		void Draw(ShaderPtr shader);

	private:
		EntityPtr _rootEntity;
	};



	template<typename Fun>
	inline void Entity::Traverse(Fun fun)
	{
		fun(*this);
		for (auto& child : _children) {
			child->Traverse(fun);
		}
	}
}