// ¯\_(ツ)_/¯

#pragma once

#include "Forward.h"

namespace MyGL
{
	class Scene : public std::enable_shared_from_this<Scene>
	{
	public:
		Scene();
		~Scene();

		EntityPtr CreateEntity(EntityPtr parentEntity = nullptr);

		void AddEntity(EntityPtr entity, EntityPtr parentEntity = nullptr);
		void RemoveEntity(EntityPtr entity);

		void AddComponent(EntityPtr entity, ComponentPtr component);
		void RemoveComponent(EntityPtr entity, ComponentPtr component);

		void Update(float dt);
		void Draw();

		void AddSystem(SystemPtr system);

		int EnsureComponentTypeId(ComponentType componentType);

	private:
		void Complete();

		void CompleteAddingEntities();
		void CompleteAddingComponents();

		void CompleteRemovingEntities();
		void CompleteRemovingComponents();

		void CompleteChangingComponentsEntities();

		void EntityAdded(EntityPtr entity);
		void ComponentAdded(EntityPtr entity, ComponentPtr component);

		void EntityRemoved(EntityPtr entity);
		void ComponentRemoved(EntityPtr entity, ComponentPtr component);

		int EnsureComponentTypeId(ComponentPtr component);

	private:
		EntityPtr _rootEntity;
		
		std::vector<EntityPtr> _addingEntities;
		std::vector<std::tuple<EntityPtr, ComponentPtr>> _addingComponents;

		std::vector<EntityPtr> _removingEntities;
		std::vector<std::tuple<EntityPtr, ComponentPtr>> _removingComponents;

		std::vector<EntityPtr> _changingComponentsEntities;

		std::vector<SystemPtr> _systems;

		std::unordered_map<ComponentType, int> _componentTypeIds;
		int _componentTypeIdCounter = 0;
	};
}