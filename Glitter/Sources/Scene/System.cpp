// ¯\_(ツ)_/¯

#include "stdafx.h"

#include "System.h"
#include "Component.h"
#include "Scene.h"
#include "Entity.h"

namespace MyGL
{
	System::System(ComponentType componentTypes)
	{
		_componentTypes.push_back(componentTypes);
	}

	System::~System()
	{
	}

	void System::AddedToScene(ScenePtr scene, int systemTypeId)
	{
		for (auto& componentType : _componentTypes) {
			_componentTypeSet.set(scene->EnsureComponentTypeId(componentType));
		}

		_systemTypeId = systemTypeId;
	}

	void System::EntityComponentsUpdated(EntityPtr entity)
	{
		if (IsProcessEntity(entity)) {
			if (!entity->_systemTypeSet.test(_systemTypeId)) {
				entity->_systemTypeSet.set(_systemTypeId);
				_entities.push_back(entity);
			}
		}
		else {
			if (entity->_systemTypeSet.test(_systemTypeId)) {
				entity->_systemTypeSet.reset(_systemTypeId);
				_entities.erase(std::remove(_entities.begin(), _entities.end(), entity), _entities.end());
			}
		}
	}

	bool System::IsProcessEntity(EntityPtr entity) const
	{
		return (entity->_componentTypeSet & _componentTypeSet) == _componentTypeSet;
	}
	std::vector<EntityPtr>& System::GetEntities()
	{
		return _entities;
	}
}
