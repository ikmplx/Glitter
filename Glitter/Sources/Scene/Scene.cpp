// ¯\_(ツ)_/¯

#include "stdafx.h"

#include "Utils.h"
#include "Scene.h"
#include "Entity.h"
#include "Component.h"
#include "System.h"
#include "Model/Mesh.h"
#include "Res/Shader.h"

namespace MyGL
{
	Scene::Scene()
		: _rootEntity(new Entity())
	{
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

	void Scene::Draw(ShaderPtr shader)
	{
		_rootEntity->Traverse([&shader](EntityPtr entity) {
			entity->Draw(shader);
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

		CompleteChangingComponentsEntities();
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

	void Scene::CompleteChangingComponentsEntities()
	{
		std::set<Entity*> uniqueSet;

		for (auto& c : _changingComponentsEntities) {
			auto iter = uniqueSet.find(c.get());
			if (iter == uniqueSet.end()) {
				for (auto& system : _systems) {
					system->EntityComponentsUpdated(c);
					uniqueSet.insert(c.get());
				}
			}
		}

		_changingComponentsEntities.clear();
	}

	void Scene::EntityAdded(EntityPtr entity)
	{
		for (auto& component : entity->_components) {
			_addingComponents.emplace_back(entity, component);
		}
	}

	void Scene::ComponentAdded(EntityPtr entity, ComponentPtr component)
	{
		int componentTypeId = EnsureComponentTypeId(component);
		if (!entity->_componentTypeSet.test(componentTypeId)) {
			entity->_componentTypeSet.set(componentTypeId);
			entity->_components.push_back(component);

			_changingComponentsEntities.push_back(entity);
		}
	}

	void Scene::EntityRemoved(EntityPtr entity)
	{
		for (auto& component : entity->_components) {
			_removingComponents.emplace_back(entity, component);
		}
	}

	void Scene::ComponentRemoved(EntityPtr entity, ComponentPtr component)
	{
		int componentTypeId = EnsureComponentTypeId(component);
		if (entity->_componentTypeSet.test(componentTypeId)) {
			entity->_componentTypeSet.reset(EnsureComponentTypeId(component));
			entity->_components.erase(std::find(entity->_components.begin(), entity->_components.end(), component), entity->_components.end());

			_changingComponentsEntities.push_back(entity);
		}
	}

	int Scene::EnsureComponentTypeId(ComponentPtr component)
	{
		if (component->componentTypeId < 0) {
			component->componentTypeId = EnsureComponentTypeId(typeid(*component));
		}

		return component->componentTypeId;
	}

	int Scene::EnsureComponentTypeId(ComponentType componentType)
	{
		auto emplaceResult = _componentTypeIds.emplace(componentType, _componentTypeIdCounter);
		if (emplaceResult.second) {
			_componentTypeIdCounter++;
		}

		return emplaceResult.first->second;
	}
}
