// ¯\_(ツ)_/¯

#pragma once

#include "Forward.h"
#include "Scene/System.h"
#include "Scene/Component.h"
#include "Scene/Entity.h"

namespace MyGL
{
	struct Light
	{
		glm::vec3 color;

		float constant;
		float linear;
		float quadratic;
	};

	class LightComponent : public Component
	{
	public:
		LightComponent(const glm::vec3& color, float constant, float linear, float quadratic);

		Light light;
	};

	class LightSystem : public System
	{
	public:
		LightSystem();

		void Update(ScenePtr scene, float dt) override;

		template <typename Fun>
		void ForEachLight(Fun fun)
		{
			for (auto& entity : GetEntities()) {
				fun(entity->FindComponent<LightComponent>()->light, entity->GetGlobalPosition());
			}
		}
	};
}
