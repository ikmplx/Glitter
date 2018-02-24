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

		EntityPtr CreateEntity();

		void AddEntity(EntityPtr ptr);
		void Draw(ShaderPtr shader);

	private:
		EntityPtr _rootEntity;
	};

}