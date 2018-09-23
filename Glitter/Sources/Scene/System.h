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

		virtual void BeforeEntityRemove(EntityPtr entity);


		void AddedToScene(ScenePtr scene, int systemTypeId);

		void EntityComponentsUpdated(EntityPtr entity);

		bool IsProcessEntity(EntityPtr entity) const;

		std::vector<EntityPtr>& GetEntities();

	private:
		std::vector<EntityPtr> _entities;
		std::vector<ComponentType> _componentTypes;

		ComponentTypeSet _componentTypeSet;

		int _systemTypeId = -1;
	};
	using SystemPtr = std::shared_ptr<System>;
}
