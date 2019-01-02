// ¯\_(ツ)_/¯

#pragma once

#include "Forward.h"

namespace MyGL
{
	class System {
	public:
		System(std::initializer_list<ComponentType> componentTypes);
		System(const System& other) = delete;
		explicit System(ComponentType componentType);

		virtual ~System();

		virtual void Update(ScenePtr scene, float dt) = 0;

		void AddedToScene(ScenePtr scene, int systemTypeId);

		std::vector<EntityPtr>& GetEntities();

	private:
		std::vector<EntityPtr> _entities;
		std::vector<ComponentType> _componentTypes;

		int _systemTypeId = -1;
	};
	using SystemPtr = std::shared_ptr<System>;
}
