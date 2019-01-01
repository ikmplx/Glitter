// ¯\_(ツ)_/¯

#pragma once

#include "Forward.h"
#include "Array2D.h"
#include "IdPool.h"

namespace MyGL
{
	struct ComponentHolder
	{
		ComponentPtr component;
	};

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

		template<typename T>
		std::shared_ptr<T> GetComponent(EntityPtr entity)
		{
			static_assert(std::is_base_of<Component, T>::value);
			return std::static_pointer_cast<T>(GetComponent(entity, Utils::TypeId<T, Component>::GetId()));
		}

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

		ComponentPtr GetComponent(EntityPtr entity, int compId);

	private:
		EntityPtr _rootEntity;
		
		std::vector<EntityPtr> _addingEntities;
		std::vector<std::tuple<EntityPtr, ComponentPtr>> _addingComponents;

		std::vector<EntityPtr> _removingEntities;
		std::vector<std::tuple<EntityPtr, ComponentPtr>> _removingComponents;

		std::vector<EntityPtr> _changingComponentsEntities;

		std::vector<SystemPtr> _systems;

		// [components][entities]
		Array2D<ComponentHolder> _table;
		IdPool _entityIdPool;
	};
}
