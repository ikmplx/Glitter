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

	private:
		EntityPtr _nanosuitPrefab;
		EntityPtr _towerPrefab;
		ScenePtr _scene;

		EntityPtr _floorEntity;
		EntityPtr _centerEntity;
		EntityPtr _nanosuitEntity1;
		EntityPtr _nanosuitEntity2;
		EntityPtr _towerEntity;

		FramebufferPtr _mainFramebuffer;
		AttachmentPtr _mainAttachment;
	};
}
