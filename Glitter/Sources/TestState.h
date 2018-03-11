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
		void CreateFramebuffer();

		void DrawSkybox();
		void DrawFullscreen();
		glm::vec3 Gamma(const glm::vec3& color);

	private:
		float _gamma = 1.0;

		EntityPtr _nanosuitPrefab;
		EntityPtr _towerPrefab;
		ScenePtr _scene;

		EntityPtr _floorEntity;
		EntityPtr _centerEntity;
		EntityPtr _nanosuitEntity1;
		EntityPtr _nanosuitEntity2;
		EntityPtr _towerEntity;

		DeferredRendererPtr _deferredRenderer;

		FramebufferPtr _framebufferPass2;
		AttachmentPtr _colorPass2;
	};
}
