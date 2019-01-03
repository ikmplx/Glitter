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

		EntityWeakPtr entity;
		int prev = -1;
		int next = -1;
	};

	class EntityRangeIterator
	{
	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = EntityPtr;
		using difference_type = int;
		using pointer = EntityPtr*;
		using reference = EntityPtr;

		EntityRangeIterator(Array2D<ComponentHolder>& table, int componentTypeId, int currentEntity)
			: _table(table)
			, _componentTypeId(componentTypeId)
			, _currentEntity(currentEntity)
		{
		}

		EntityRangeIterator& operator++()  // pre
		{
			auto& c = _table[_componentTypeId][_currentEntity];
			_currentEntity = c.next;
			return *this;
		}

		EntityRangeIterator operator++(int)  // post
		{
			auto ret = *this;
			++(*this);
			return ret;
		}

		bool operator==(EntityRangeIterator other) const
		{
			return _currentEntity == other._currentEntity;
		}

		bool operator!=(EntityRangeIterator other) const
		{
			return !(*this == other);
		}

		EntityPtr operator*() const
		{
			return _table[_componentTypeId][_currentEntity].entity.lock();
		}

	private:
		Array2D<ComponentHolder>& _table;
		int _componentTypeId;
		int _currentEntity;
	};

	class EntityRange
	{
	public:
		EntityRange(Array2D<ComponentHolder>& table, int componentTypeId, int beginning)
			: _table(table)
			, _componentTypeId(componentTypeId)
			, _beginning(beginning)
		{
		}

		EntityRangeIterator begin() const
		{
			return EntityRangeIterator(_table, _componentTypeId, _beginning);
		}

		EntityRangeIterator end() const
		{
			return EntityRangeIterator(_table, _componentTypeId, -1);
		}

	private:
		Array2D<ComponentHolder>& _table;
		int _componentTypeId;
		int _beginning;
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

		template<typename T>
		EntityRange ForEachEntity()
		{
			auto componentTypeId = Utils::TypeId<T, Component>::GetId();
			return EntityRange(_table, componentTypeId, GetComponentListHead(componentTypeId));
		}

		template<typename T, typename Fun>
		void ForEachEntity(Fun fun)
		{
			for (auto& entity : ForEachEntity<T>()) {
				fun(entity);
			}
		}

	private:
		void Complete();

		void CompleteAddingEntities();
		void CompleteAddingComponents();

		void CompleteRemovingEntities();
		void CompleteRemovingComponents();

		void EntityAdded(EntityPtr entity);
		void ComponentAdded(EntityPtr entity, ComponentPtr component);

		void EntityRemoved(EntityPtr entity);
		void ComponentRemoved(EntityPtr entity, ComponentPtr component);

		ComponentPtr GetComponent(EntityPtr entity, int compId);

		int& GetComponentListHead(const ComponentPtr& component);
		int& GetComponentListHead(int typeId);

		void InsertComponentToList(ComponentHolder& componentHolder, int entityId);
		void RemoveComponentFromList(ComponentHolder& componentHolder);

	private:
		EntityPtr _rootEntity;
		
		std::vector<EntityPtr> _addingEntities;
		std::vector<std::tuple<EntityPtr, ComponentPtr>> _addingComponents;

		std::vector<EntityPtr> _removingEntities;
		std::vector<std::tuple<EntityPtr, ComponentPtr>> _removingComponents;

		std::vector<SystemPtr> _systems;

		// [components][entities]
		Array2D<ComponentHolder> _table;
		IdPool _entityIdPool;
		std::vector<int> _componentLists;
	};
}
