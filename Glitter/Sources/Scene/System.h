// ¯\_(ツ)_/¯

#pragma once

#include "Forward.h"

namespace MyGL
{
	class System {
	public:
		System(ComponentType componentTypes);
		virtual ~System();

		virtual void Update(float dt) = 0;

		void AddedToScene(ScenePtr scene, int systemTypeId);

		void EntityComponentsUpdated(EntityPtr entity);

	private:

	private:
		std::vector<EntityPtr> _entities;
		std::vector<ComponentType> _componentTypes;

		ComponentTypeSet _componentTypeSet;

		int _systemTypeId;
	};
	using SystemPtr = std::shared_ptr<System>;
}
