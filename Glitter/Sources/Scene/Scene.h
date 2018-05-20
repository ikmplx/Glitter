// ¯\_(ツ)_/¯

#pragma once

#include "Forward.h"

namespace MyGL
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		EntityPtr CreateEntity(EntityPtr parentEntity = nullptr);
		void AddEntity(EntityPtr entity, EntityPtr parentEntity = nullptr);

		void Update(float dt);
		void Draw(ShaderPtr shader);

	private:
		void CompleteAddingEntities();

	private:
		EntityPtr _rootEntity;
		std::vector<EntityPtr> _addingEntities;
	};
}