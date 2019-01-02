// ¯\_(ツ)_/¯

#include "stdafx.h"

#include "Utils.h"
#include "Scene.h"
#include "Entity.h"
#include "Component.h"
#include "System.h"

namespace MyGL
{
	Scene::Scene()
		: _rootEntity(new Entity())
	{
		_rootEntity->SetId(_entityIdPool.Alloc());

		// _table.SafeRef(1000, 1000);
	}

	Scene::~Scene() = default;

	EntityPtr Scene::CreateEntity(EntityPtr parentEntity)
	{
		EntityPtr entity = std::make_shared<Entity>();
		AddEntity(entity, parentEntity);
		return entity;
	}

	void Scene::AddEntity(EntityPtr entity, EntityPtr parentEntity)
	{
		if (!parentEntity) {
			parentEntity = _rootEntity;
		}
		entity->_parent = parentEntity->weak_from_this();

		_addingEntities.push_back(entity);
	}

	void Scene::RemoveEntity(EntityPtr entity)
	{
		_removingEntities.push_back(entity);
	}

	void Scene::AddComponent(EntityPtr entity, ComponentPtr component)
	{
		_addingComponents.emplace_back(entity, component);
	}

	void Scene::RemoveComponent(EntityPtr entity, ComponentPtr component)
	{
		_removingComponents.emplace_back(entity, component);
	}

	void Scene::Update(float dt)
	{
		ScenePtr sharedThis = shared_from_this();

		Complete();

		for (auto& system : _systems) {
			system->Update(sharedThis, dt);
		}

		Complete();
	}

	void Scene::Draw()
	{
		ImGui::Text("%d", _table.SizeY());
		_rootEntity->Traverse([](EntityPtr entity) {
			entity->Draw();
		});
	}

	void Scene::AddSystem(SystemPtr system)
	{
		system->AddedToScene(shared_from_this(), (int) _systems.size());
		_systems.push_back(system);
	}

	void Scene::Complete()
	{
		CompleteAddingEntities();
		CompleteAddingComponents();

		CompleteRemovingEntities();
		CompleteRemovingComponents();
	}

	void Scene::CompleteAddingEntities()
	{
		for (auto& addingEntity : _addingEntities) {
			EntityPtr parent = addingEntity->_parent.lock();
			if (parent) {
				parent->_children.push_back(addingEntity);
			}

			addingEntity->Traverse([this](EntityPtr ent) {
				EntityAdded(ent);
			});
		}

		_addingEntities.clear();
	}

	void Scene::CompleteAddingComponents()
	{
		for (auto& addingComponentTuple : _addingComponents) {
			EntityPtr entity = std::get<0>(addingComponentTuple);
			ComponentPtr component = std::get<1>(addingComponentTuple);
			ComponentAdded(entity, component);
		}

		_addingComponents.clear();
	}

	void Scene::CompleteRemovingEntities()
	{
		for (auto& removingEntity : _removingEntities) {
			EntityPtr parent = (removingEntity->_parent).lock();
			if (parent) {
				auto& children = parent->_children;

				// TODO: OPTIMIZATION: can erase all children at once
				children.erase(std::remove(children.begin(), children.end(), removingEntity), children.end());
				removingEntity->_parent.reset();

				removingEntity->Traverse([this](EntityPtr ent) {
					EntityRemoved(ent);
				});
			}
		}

		_removingEntities.clear();
	}

	void Scene::CompleteRemovingComponents()
	{
		for (auto& removingComponentTuple : _removingComponents) {
			EntityPtr entity = std::get<0>(removingComponentTuple);
			ComponentPtr component = std::get<1>(removingComponentTuple);
			ComponentRemoved(entity, component);
		}

		_removingComponents.clear();
	}

	void Scene::EntityAdded(EntityPtr entity)
	{
		MyAssert(entity->GetId() < 0);
		entity->SetId(_entityIdPool.Alloc());

		_table.SafeRef(0, entity->GetId());

		for (auto& component : entity->_components) {
			_addingComponents.emplace_back(entity, component);
		}
	}

	void Scene::ComponentAdded(EntityPtr entity, ComponentPtr component)
	{
		int componentTypeId = component->GetTypeId();
		
		// New
		auto& c = _table.SafeRef(componentTypeId, entity->GetId());
		MyAssert(!c.component);
		MyAssert(c.next == -1);
		MyAssert(c.prev == -1);
		c.component = component;
		c.entity = entity;
		c.component->attached = true;

		InsertComponentToList(c, entity->GetId());
	}

	void Scene::EntityRemoved(EntityPtr entity)
	{
		// Deleting all entity's components
		for (int i = 0; i < _table.SizeX(); i++) {
			auto& c = _table.SafeRef(i, entity->GetId());
			if (c.component != nullptr) {
				RemoveComponentFromList(c);
				c.component->attached = false;
				c.component = nullptr;
			}
		}

		// Deleing entity
		_entityIdPool.Dealloc(entity->GetId());
		entity->SetId(-1);
	}

	void Scene::ComponentRemoved(EntityPtr entity, ComponentPtr component)
	{
		if (entity->GetId() >= 0) {
			auto& c = _table.SafeRef(component->GetTypeId(), entity->GetId());
			MyAssert(c.component != nullptr);

			RemoveComponentFromList(c);
			c.component->attached = false;
			c.component = nullptr;
			c.entity.reset();
		}
	}

	ComponentPtr Scene::GetComponent(EntityPtr entity, int compId)
	{
		auto comp = _table[compId][entity->GetId()].component;
		MyAssert(comp == nullptr || comp->GetTypeId() == compId);
		return comp;
	}

	int& Scene::GetComponentListHead(const ComponentPtr& component)
	{
		return GetComponentListHead(component->GetTypeId());
	}

	int& Scene::GetComponentListHead(int typeId)
	{
		_componentLists.resize(std::max<size_t>(typeId + size_t(1), _componentLists.size()), -1);
		return _componentLists[typeId];
	}

	void Scene::InsertComponentToList(ComponentHolder& componentHolder, int entityId)
	{
		auto compTypeId = componentHolder.component->GetTypeId();
		auto& head = GetComponentListHead(compTypeId);

		componentHolder.next = head;
		componentHolder.prev = -1;
		head = entityId;

		if (componentHolder.next != -1) {
			_table[compTypeId][componentHolder.next].prev = entityId;
		}
	}

	void Scene::RemoveComponentFromList(ComponentHolder& componentHolder)
	{
		auto compTypeId = componentHolder.component->GetTypeId();

		auto prev = componentHolder.prev;
		auto next = componentHolder.next;

		if (prev != -1) {
			_table[compTypeId][prev].next = next;
		} else {
			auto& head = GetComponentListHead(compTypeId);
			head = next;
		}

		if (next != -1) {
			_table[compTypeId][next].prev = prev;
		}

		componentHolder.next = componentHolder.prev = -1;
	}
}
