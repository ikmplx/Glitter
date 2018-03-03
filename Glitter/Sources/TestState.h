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

		virtual void Update(float dt) override;
		virtual void Draw() override;

	private:
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

		MeshPtr _skyboxCube;
	};
}
