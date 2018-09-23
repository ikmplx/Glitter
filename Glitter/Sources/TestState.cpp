// ¯\_(ツ)_/¯

#include "stdafx.h"

#include "TestState.h"

#include "Res/Shader.h"
#include "Res/Texture.h"
#include "Model/Mesh.h"
#include "Model/Model.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/Camera.h"
#include "Framebuffer.h"
#include "DeferredRenderer.h"
#include "Model/Material.h"
#include "Physics.h"
#include "Scene/System.h"
#include "Scene/Component.h"

#include "Utils.h"

namespace MyGL
{
	class TestComponent : public Component
	{
	};

	class TestSystem : public System
	{
	public:
		TestSystem()
			: System(typeid(TestComponent))
		{
		}

		virtual void Update(ScenePtr scene, float dt) override
		{
			for (auto& entity : GetEntities()) {
				if (entity->GetGlobalPosition().y < -10) {
					scene->RemoveEntity(entity);
				}
			}
		}
	};

	TestState::TestState()
	{
	}

	TestState::~TestState()
	{
	}

	void TestState::CreatePhysicObjects()
	{
		// Plane
		_floorEntity = _scene->CreateEntity(_floorEntity);

		auto floorPlaneMesh = Primitives::CreatePlane(100.f, 100.f, 0.1f);
		_floorEntity->SetMesh(floorPlaneMesh);
		_floorEntity->SetMaterial(std::make_shared<Material>(ResourceManager::Instance()->GetTexture("Wood")));
		_floorEntity->GetMaterial()->specularBase = 0.3f;

		btBoxShape* floorShape = new btBoxShape(btVector3(50.f, 0.1f, 50.f));
		RigidBodyPtr floorBody = std::make_shared<RigidBody>(_floorEntity, floorShape, 0.f);
		_floorEntity->SetRigidBody(floorBody);
		Physics::Instance()->AddRigidBody(floorBody);

		// Box
		btBoxShape* boxShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
		for (int i = 0; i < 100; i++) {
			auto boxEntity = _scene->CreateEntity();
			_scene->AddComponent(boxEntity, ComponentPtr(new TestComponent()));

			auto boxMesh = Primitives::CreateCube();
			boxEntity->SetMesh(boxMesh);
			boxEntity->SetMaterial(std::make_shared<Material>(ResourceManager::Instance()->GetTexture("Awesome")));

			RigidBodyPtr boxBody = std::make_shared<RigidBody>(boxEntity, boxShape, 1.f);
			boxEntity->SetRigidBody(boxBody);
			Physics::Instance()->AddRigidBody(boxBody);

			boxEntity->position = glm::vec3(0.f, 10.f + i * 1.5f, 12.f);
			boxEntity->rotation = glm::angleAxis(1.1f + i, glm::normalize(glm::vec3(1.f, 0.9f, 0.8f)));
		}
	}

	void TestState::Init()
	{
		GetCamera().SetPosition(glm::vec3(0.f, 7.f, 37.f));

		//_nanosuitPrefab = ModelLoader::LoadModel("Models/nanosuit/nanosuit.obj");

		{
			ModelLoader loader;
			loader.SetTransform(glm::rotate(glm::mat4(1.f), glm::radians(-90.f), glm::vec3(1, 0, 0)));
			_towerPrefab = loader.Load("Models/vox/chr_sword.ply");

			_towerPrefab->Traverse([](EntityPtr entity) {
				if (entity->GetMesh()) {
					entity->GetMaterial()->specularBase = 0.5f;
				}
			});
		}

		_scene = std::make_shared<MyGL::Scene>();
		_scene->AddSystem(std::make_shared<TestSystem>());

		_centerEntity = _scene->CreateEntity();

		/*
		_nanosuitEntity1 = _nanosuitPrefab->Clone();
		_nanosuitEntity2 = _nanosuitPrefab->Clone();

		_nanosuitEntity1->position += glm::vec3(-10, 0, 0);
		_nanosuitEntity2->position += glm::vec3(10, 0, 0);

		_nanosuitEntity1->rotation = glm::angleAxis(glm::radians(35.f), glm::vec3(0, 1, 0));
		_nanosuitEntity2->rotation = glm::angleAxis(glm::radians(-35.f), glm::vec3(0, 1, 0));
		*/
		_towerEntity = _towerPrefab->Clone();
		_towerEntity->position += glm::vec3(0, 0, -10);

		//_scene->AddEntity(_nanosuitEntity1, _centerEntity);
		//_scene->AddEntity(_nanosuitEntity2, _centerEntity);
		_scene->AddEntity(_towerEntity, _centerEntity);

		_deferredRenderer = std::make_shared<DeferredRenderer>(_vpWidth, _vpHeight);
		_framebufferPass2 = std::make_shared<Framebuffer>();
		_framebufferPass2->AttachColor(_colorPass2 = std::make_shared<Attachment>(_vpWidth, _vpHeight, Attachment::Type::RGB, true));
		_framebufferPass2->AttachDepth(_deferredRenderer->GetDepth());

		Physics::Initialize();
		CreatePhysicObjects();
	}

	void TestState::Deinit()
	{
		Physics::Deinitialize();
	}

	void TestState::Update(float dt)
	{
		Physics::Instance()->Update(dt);

		const float secondsPerTick = 1.f / 60.f;

		_gameTimeAcc += dt;
		while (_gameTimeAcc > secondsPerTick) {
			_gameTimeAcc -= secondsPerTick;
			UpdateGame(secondsPerTick);
		}
	}

	void TestState::UpdateGame(float dt)
	{
		_lightRotation += dt * 0.01f;
		glm::vec3 lightDir = glm::vec3(0.2f, -0.8f, -1.f);
		glm::quat lightRotation = glm::angleAxis(_lightRotation, glm::vec3(0.f, 1.f, 0.f));
		_lightDir = lightRotation * lightDir;

		_scene->Update(dt);
	}

	void TestState::Draw()
	{
		float pulseProgress = 0.75f + 0.25f * (float)sin(glfwGetTime());
		float constantProgress = (float)fmod(glfwGetTime(), 100.0) * 4.f;

		// _centerEntity->position = glm::vec3(0.f, 0.f, -constantProgress * 10.f);
		// _centerEntity->rotation = glm::angleAxis(constantProgress / 2.f, glm::vec3(0, 1, 0));
		// _centerEntity->InvalidateTransform();

		glm::mat4 view = GetCamera().GetViewMatrix();
		glm::mat4 proj = glm::perspective(glm::radians(45.f), (float)_vpWidth / _vpHeight, 1.f, 500.f);

		{
			glm::mat4 combined = proj * view;
			glm::mat4 combinedNoDir = glm::mat4(glm::mat3(combined));

			MyGL::UboManager::SetMatrix(MyGL::UboManager::BINDING_MATRICES, 0, proj);
			MyGL::UboManager::SetMatrix(MyGL::UboManager::BINDING_MATRICES, 1, view);
			MyGL::UboManager::SetMatrix(MyGL::UboManager::BINDING_MATRICES, 2, combined);

			MyGL::UboManager::SetMatrix(MyGL::UboManager::BINDING_MATRICES_EXT, 0, combinedNoDir);
			MyGL::UboManager::SetMatrix(MyGL::UboManager::BINDING_MATRICES_EXT, 1, glm::inverse(combined));
			MyGL::UboManager::SetMatrix(MyGL::UboManager::BINDING_MATRICES_EXT, 2, glm::inverse(combinedNoDir));

			MyGL::UboManager::SetVector(MyGL::UboManager::BINDING_VECTORS, 0, glm::vec4(GetCamera().GetPosition(), 0.f));
		}

		ImGui::SliderFloat("Gamma", &_gamma, 1.f, 2.4f);

		// Pass 1
		_deferredRenderer->BeginPass1();

		auto shader = MyGL::ResourceManager::Instance()->GetShader("Pass1");
		shader->Bind();
		shader->SetFloat("gamma", _gamma);
		_scene->Draw(shader);

		_deferredRenderer->EndPass1();

		// Pass 2
		_framebufferPass2->BeginRender();
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);

		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glClearColor(0.f, 0.f, 0.f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Ambient
		ShaderPtr ambientShader = ResourceManager::Instance()->GetShader("Pass2Ambient");
		ambientShader->Bind();
		ambientShader->SetVec3("ambientLight.color", Gamma(glm::vec3(0.15f)));

		_deferredRenderer->BindColorAttachments(ambientShader);
		DrawFullscreen();

		// Directional
		ShaderPtr dirShader = ResourceManager::Instance()->GetShader("Pass2Directional");
		dirShader->Bind();

		glm::vec3 lightDir = _lightDir;
		// glm::quat lightRotation = glm::angleAxis(_lightRotation, glm::vec3(0.f, 1.f, 0.f));
		// lightDir = lightRotation * lightDir;

		static bool enableBlinn = true;
		ImGui::Checkbox("Blinn", &enableBlinn);

		dirShader->SetVec3("dirLight.color", Gamma(glm::vec3(0.7f, 0.7f, 0.7f)));
		dirShader->SetVec3("dirLight.direction", lightDir);
		dirShader->SetFloat("dirLight.ambient", 0.f);
		dirShader->SetInt("enableBlinn", enableBlinn);

		_deferredRenderer->BindColorAttachments(dirShader);
		DrawFullscreen();

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		// Forward
		DrawSkybox();

		_framebufferPass2->EndRender();

		if (_gamma > 1.f) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glDisable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);

			ShaderPtr gammaShader = ResourceManager::Instance()->GetShader("Gamma");
			gammaShader->Bind();
			gammaShader->SetFloat("gamma", _gamma);

			glActiveTexture(GL_TEXTURE0);
			_colorPass2->Bind();
			gammaShader->SetInt("texture1", 0);

			DrawFullscreen();
		}
		else {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, _framebufferPass2->GetId());
			glBlitFramebuffer(0, 0, _vpWidth, _vpHeight, 0, 0, _vpWidth, _vpHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void TestState::CreateFramebuffer()
	{
	}

	void TestState::DrawSkybox() {
		MyGL::CubemapPtr skyboxTexture = MyGL::ResourceManager::Instance()->GetCubemap("Skybox");
		MyGL::ShaderPtr shader = MyGL::ResourceManager::Instance()->GetShader("Skybox");

		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);

		glActiveTexture(GL_TEXTURE0);
		skyboxTexture->Bind();

		glm::mat4 skyTransform(1.f);
		//skyTransform = glm::scale(skyTransform, glm::vec3(1.f, 1.f, -1.f));

		shader->Bind();
		shader->SetInt("sampler", 0);
		shader->SetFloat("gamma", _gamma);
		shader->SetMatrix("skyTransform", skyTransform);
		shader->SetVec3("sunDir", glm::normalize(-_lightDir));

		glBindVertexArray(_emptyVao);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);
	}

	void TestState::DrawFullscreen()
	{
		glBindVertexArray(_emptyVao);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	glm::vec3 TestState::Gamma(const glm::vec3 & color)
	{
		return glm::pow(color, glm::vec3(_gamma));
	}
}
