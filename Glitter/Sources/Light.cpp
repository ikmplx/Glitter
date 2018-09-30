// ¯\_(ツ)_/¯

#include "stdafx.h"

#include "Light.h"

namespace MyGL
{
	LightComponent::LightComponent(const glm::vec3& color, float constant, float linear, float quadratic)
		: light({color, constant, linear, quadratic})
	{
	}

	LightSystem::LightSystem()
		: System(typeid(LightComponent))
	{
	}

	void LightSystem::Update(ScenePtr scene, float dt)
	{
	}
}
