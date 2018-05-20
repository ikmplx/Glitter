// ¯\_(ツ)_/¯

#include "stdafx.h"

#include "Scene.h"
#include "Entity.h"
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

	void Scene::CompleteAddingEntities()
	{
		for (auto& addingEntity : _addingEntities) {
			EntityPtr parent = addingEntity->_parent.lock();
			if (parent) {
				parent->_children.push_back(addingEntity);
			}
		}

		_addingEntities.clear();
	}
}

