// ¯\_(ツ)_/¯

#include "stdafx.h"

#include "System.h"
#include "Component.h"
#include "Scene.h"
#include "Entity.h"

namespace MyGL
{
	System::System(std::initializer_list<ComponentType> componentTypes)
	{
		_componentTypes.assign(componentTypes.begin(), componentTypes.end());
	}

	System::System(ComponentType componentTypes)
	{
		_componentTypes.push_back(componentTypes);
	}

	System::~System() = default;

	void System::AddedToScene(ScenePtr scene, int systemTypeId)
	{
		_systemTypeId = systemTypeId;
	}

	std::vector<EntityPtr>& System::GetEntities()
	{
		return _entities;
	}
}
