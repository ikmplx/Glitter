// ¯\_(ツ)_/¯

#pragma once

#include "GameState.h"

namespace MyGL
{
	class TestState : public GameState
	{
	public:
		TestState();
		~TestState();

		virtual void Init() override;
		virtual void Deinit() override;

		virtual void Update(float dt) override;
		virtual void Draw() override;

	private:
		void UpdateGame(float dt);

		void CreateFramebuffer();
		void CreatePhysicObjects();

		void DrawSkybox();
		void DrawFullscreen();
		glm::vec3 Gamma(const glm::vec3& color);

	private:
		float _gamma = 2.2f;

		EntityPtr _floorEntity;
		std::vector<EntityPtr> _boxes;

		EntityPtr _nanosuitPrefab;
		EntityPtr _towerPrefab;
		ScenePtr _scene;

		EntityPtr _centerEntity;
		EntityPtr _nanosuitEntity1;
		EntityPtr _nanosuitEntity2;
		EntityPtr _towerEntity;

		DeferredRendererPtr _deferredRenderer;

		FramebufferPtr _framebufferPass2;
		AttachmentPtr _colorPass2;

		float _gameTimeAcc = 0.f;

		float _lightRotation = 0.f;
		glm::vec3 _lightDir = glm::vec3(0.2f, -1.f, -1.f);
	};
}
