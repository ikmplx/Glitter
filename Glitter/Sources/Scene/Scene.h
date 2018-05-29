// ¯\_(ツ)_/¯

#pragma once

#include "Forward.h"

namespace MyGL
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		EntityPtr CreateEntity(EntityPtr parentEntity = nullptr);
		void AddEntity(EntityPtr entity, EntityPtr parentEntity = nullptr);

		void AddComponent(EntityPtr entity, ComponentPtr component);
		void RemoveComponent(EntityPtr entity, ComponentPtr component);

		void Update(float dt);
		void Draw(ShaderPtr shader);

		void AddSystem(SystemPtr system);


	private:
		void CompleteAddingEntities();

		void EntityAdded(EntityPtr entity);

		void EnsureComponentTypeId(ComponentPtr component);

	private:
		EntityPtr _rootEntity;
		std::vector<EntityPtr> _addingEntities;

		std::vector<SystemPtr> _systems;

		std::unordered_map<ComponentType, int> _componentTypeIds;
		int _componentTypeIdCounter = 0;
	};
}