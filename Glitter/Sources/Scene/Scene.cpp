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

	void Scene::Update(float dt)
	{

		CompleteAddingEntities();
	}

	void Scene::Draw(ShaderPtr shader)
	{
		_rootEntity->Traverse([&shader](Entity& entity) {
			entity.Draw(shader);
		});
	}

	void Scene::AddSystem(SystemPtr system)
	{
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

	void Scene::EntityAdded(EntityPtr entity)
	{
		for (auto& system : _systems) {
			system->EntityAdded(entity);
		}
	}

	void Scene::EnsureComponentTypeId(ComponentPtr component)
	{
		if (component->componentTypeId < 0) {
			ComponentType ct(typeid(*component));

			auto emplaceResult = _componentTypeIds.emplace(ct, _componentTypeIdCounter);
			if (emplaceResult.second) {
				_componentTypeIdCounter++;
			}

			component->componentTypeId = emplaceResult.first->second;
		}
	}
}

