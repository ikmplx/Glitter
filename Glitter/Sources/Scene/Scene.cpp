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

	void Scene::AddComponent(EntityPtr entity, ComponentPtr component)
	{
		_addingComponents.emplace_back(entity, component);
	}

	void Scene::Update(float dt)
	{
		CompleteAddingEntities();
		CompleteAddingComponents();
		CompleteChangingComponentsEntities();


		for (auto& system : _systems) {
			system->Update(dt);
		}


		CompleteAddingEntities();
		CompleteAddingComponents();
		CompleteChangingComponentsEntities();
	}

	void Scene::Draw(ShaderPtr shader)
	{
		_rootEntity->Traverse([&shader](Entity& entity) {
			entity.Draw(shader);
		});
	}

	void Scene::AddSystem(SystemPtr system)
	{
		system->AddedToScene(shared_from_this(), (int) _systems.size());
		_systems.push_back(system);
	}

	void Scene::CompleteAddingEntities()
	{
		for (auto& addingEntity : _addingEntities) {
			EntityPtr parent = addingEntity->_parent.lock();
			if (parent) {
				parent->_children.push_back(addingEntity);
			}

			addingEntity->Traverse([this](Entity& ent) {
				EntityAdded(ent.shared_from_this());
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

	void Scene::CompleteChangingComponentsEntities()
	{
		for (auto& c : _changingComponentsEntities) {
			for (auto& system : _systems) {
				system->EntityComponentsUpdated(c);
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
		entity->_componentTypeSet.set(EnsureComponentTypeId(component));
		_changingComponentsEntities.insert(entity);
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
