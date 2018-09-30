// ¯\_(ツ)_/¯

#pragma once

#include "GameState.h"

namespace MyGL
{
	class LightSystem;

	class TestState : public GameState
	{
	public:
		TestState();
		~TestState();

		virtual void Init() override;
		virtual void Deinit() override;

		virtual void Update(float dt) override;
		virtual void Draw() override;

		virtual void MouseDown(int button, float x, float y) override;
		virtual void MouseUp(int button, float x, float y) override;


	private:
		void UpdateGame(float dt);

		void CreateFramebuffer();
		void CreatePhysicObjects();

		void DrawSkybox();
		void DrawFullscreen(ShaderPtr& shader);
		glm::vec3 Gamma(const glm::vec3& color);

		glm::mat4 GetProjMatrix() const;
		glm::vec3 CameraRay(float x, float y) const;

	private:
		bool _enableBlinn = true;
		float _gamma = 2.2f;
		bool _debugGBuffer = false;

		EntityPtr _floorEntity;

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

		float _dropTimer = 0.1f;
		int _dropCount = 100;

		std::unique_ptr<btBoxShape> _boxShape = std::make_unique<btBoxShape>(btVector3(0.5f, 0.5f, 0.5f));
		TriangleShapeHolderPtr _testShape;

		MeshPtr _sphereMesh;
		std::unique_ptr<btSphereShape> _sphereShape = std::make_unique<btSphereShape>(0.6f);

		std::shared_ptr<LightSystem> _lightSystem;
		MeshPtr _lightSphereMesh;
	};
}
