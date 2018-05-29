// ¯\_(ツ)_/¯

#include "stdafx.h"

#include "System.h"
#include "Component.h"

namespace MyGL
{
	System::System(ComponentType componentTypes)
	{
		_componentTypes.push_back(componentTypes);

		UpdateComponentTypesSet();
	}

	System::~System()
	{
	}

	void System::Update(float dt)
	{
	}

	void System::EntityAdded(EntityPtr entity)
	{
	}

	void System::ComponentAdded(EntityPtr entity, ComponentPtr component)
	{
	}

	void System::UpdateComponentTypesSet()
	{
	}

}
