// ¯\_(ツ)_/¯

#pragma once

#include "Forward.h"

namespace MyGL
{
	class System {
	public:
		System(ComponentType componentTypes);
		virtual ~System();

		virtual void Update(float dt);

		void EntityAdded(EntityPtr entity);
		void EntityRemoved(EntityPtr entity);

		void ComponentAdded(EntityPtr entity, ComponentPtr component);
		void ComponentRemoved(EntityPtr entity, ComponentPtr component);

	private:
		void UpdateComponentTypesSet();

	private:
		std::vector<EntityPtr> _entities;
		std::vector<ComponentType> _componentTypes;

	};
	using SystemPtr = std::shared_ptr<System>;
}
