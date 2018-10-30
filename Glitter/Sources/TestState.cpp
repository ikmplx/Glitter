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
#include "StateManager.h"
#include "Light.h"

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
				if (entity->GetGlobalPosition().y < -100) {
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

		auto floorPlaneMesh = Primitives::CreatePlane(50.f, 50.f, 0.1f);
		_floorEntity->SetMesh(floorPlaneMesh);

		auto material = std::make_shared<StandardMaterial>(ResourceManager::Instance()->GetTexture("Brickwall"));
		material->specularBase = 0.3f;
		material->normal = ResourceManager::Instance()->GetTexture("BrickwallNormal");

		_floorEntity->SetMaterial(material);

		btBoxShape* floorShape = new btBoxShape(btVector3(25.f, 0.1f, 25.f));
		ComponentPtr physicsComponent = std::make_shared<PhysicsComponent>(floorShape, 0.f);

		_scene->AddComponent(_floorEntity, physicsComponent);

		// Tower
		_towerEntity = _towerPrefab->Clone();
		_towerEntity->position += glm::vec3(0, 0, 0);

		_scene->AddEntity(_towerEntity, _centerEntity);

		ComponentPtr towerPhysicsComponent = std::make_shared<PhysicsComponent>(_testShape->shape.get(), 0.f);
		_scene->AddComponent(_towerEntity, towerPhysicsComponent);
	}

	void TestState::Init()
	{
		GetCamera().SetPosition(glm::vec3(0.f, 7.f, 37.f));

		//_nanosuitPrefab = ModelLoader::LoadModel("Models/nanosuit/nanosuit.obj");

		_sphereMesh = Primitives::CreateSphere(0.6f, 3);
		_lightSphereMesh = Primitives::CreateSphere(1.f, 2);
		_boxMesh = Primitives::CreateCube();

		{
			ModelLoader loader;
			loader.SetTransform(glm::rotate(glm::mat4(1.f), glm::radians(-90.f), glm::vec3(1, 0, 0)));
			loader.SpecularBase(0.5f);
			_towerPrefab = loader.Load("Models/vox/chr_sword.ply");
			_towerPrefab->Traverse([](EntityPtr& entity) {
				if (entity->GetMaterial()) {
					entity->SetMaterial(std::make_shared<VertexColorMaterial>());
				}	
			});

			_testShape = PhysicsSystem::CreateTriangleShape(_towerPrefab);
		}

		_scene = std::make_shared<MyGL::Scene>();
		_scene->AddSystem(std::make_shared<TestSystem>());
		_scene->AddSystem(std::make_shared<PhysicsSystem>());
		_scene->AddSystem(_lightSystem = std::make_shared<LightSystem>());

		_centerEntity = _scene->CreateEntity();

		/*
		_nanosuitEntity1 = _nanosuitPrefab->Clone();
		_nanosuitEntity2 = _nanosuitPrefab->Clone();

		_nanosuitEntity1->position += glm::vec3(-10, 0, 0);
		_nanosuitEntity2->position += glm::vec3(10, 0, 0);

		_nanosuitEntity1->rotation = glm::angleAxis(glm::radians(35.f), glm::vec3(0, 1, 0));
		_nanosuitEntity2->rotation = glm::angleAxis(glm::radians(-35.f), glm::vec3(0, 1, 0));
		*/

		//_scene->AddEntity(_nanosuitEntity1, _centerEntity);
		//_scene->AddEntity(_nanosuitEntity2, _centerEntity);

		_deferredRenderer = std::make_shared<DeferredRenderer>(_vpWidth, _vpHeight);
		_framebufferPass2 = std::make_shared<Framebuffer>();
		_framebufferPass2->AttachColor(_colorPass2 = std::make_shared<Attachment>(_vpWidth, _vpHeight, Attachment::Type::RGB, true));
		_framebufferPass2->AttachDepth(_deferredRenderer->GetDepth());

		CreatePhysicObjects();
	}

	void TestState::Deinit()
	{
	}

	void TestState::Update(float dt)
	{
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

		_dropTimer -= dt;
		while (_dropTimer <= 0 && _dropCount-- > 0) {
			_dropTimer += 0.02f;

			// Base box entity with physics and deferred rendered shape
			auto boxEntity = _scene->CreateEntity();
			_scene->AddComponent(boxEntity, std::make_shared<TestComponent>());

			boxEntity->SetMesh(_boxMesh);
			boxEntity->SetMaterial(std::make_shared<StandardMaterial>(ResourceManager::Instance()->GetTexture("Awesome")));

			ComponentPtr physicsComponent = std::make_shared<PhysicsComponent>(_boxShape.get(), 1.f);
			_scene->AddComponent(boxEntity, physicsComponent);

			boxEntity->position = glm::vec3(_dropCount % 6 - 3, 80.f, _dropCount % 6 - 3);
			boxEntity->rotation = glm::angleAxis(1.1f + _dropCount, glm::normalize(glm::vec3(1.f, 0.9f, 0.8f)));

			auto lightComponent = std::make_shared<LightComponent>(glm::vec3(1.f, 1.f, 0.6f), 1.f, 0.7f, 1.8f);
			_scene->AddComponent(boxEntity, lightComponent);
		}
	}

	void TestState::Pass1()
	{
		_deferredRenderer->BeginPass1();
		_scene->Draw();
		_deferredRenderer->EndPass1();
	}

	void TestState::Pass2()
	{
		_framebufferPass2->BeginRender();

		glClearColor(0.f, 0.f, 0.f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Ambient
		ShaderPtr ambientShader = ResourceManager::Instance()->GetShader("Pass2Ambient");
		ambientShader->Bind();
		ambientShader->SetVec3("ambientLight.color", Gamma(glm::vec3(0.05f)));

		_deferredRenderer->BindColorAttachments(ambientShader);

		StateManager::Instance()->SetState(StateType::LightPass);
		DrawFullscreen(ambientShader);

		// Directional
		ShaderPtr dirShader = ResourceManager::Instance()->GetShader("Pass2Directional");
		dirShader->Bind();

		dirShader->SetVec3("dirLight.color", Gamma(glm::vec3(0.15f, 0.15f, 0.15f)));
		dirShader->SetVec3("dirLight.direction", _lightDir);
		dirShader->SetFloat("dirLight.ambient", 0.f);
		dirShader->SetInt("enableBlinn", _enableBlinn);

		_deferredRenderer->BindColorAttachments(dirShader);

		StateManager::Instance()->SetState(StateType::LightPass);
		DrawFullscreen(dirShader);

		// Point light
		ShaderPtr pointShader = ResourceManager::Instance()->GetShader("Pass2Point");
		pointShader->Bind();
		pointShader->SetInt("enableBlinn", _enableBlinn);
		_deferredRenderer->BindColorAttachments(pointShader);
		glCullFace(GL_FRONT);

		_lightSystem->ForEachLight([this, &pointShader](Light& light, const glm::vec3& pos) {
			pointShader->SetVec3("pointLight.color", light.color);
			pointShader->SetFloat("pointLight.linear", light.linear);
			pointShader->SetFloat("pointLight.quadratic", light.quadratic);
			pointShader->SetFloat("pointLight.constant", light.constant);
			pointShader->SetVec3("pointLight.position", pos);

			const float minBrightness = 3.f;
			const float brightness = std::max(std::max(light.color.r, light.color.g), light.color.b);
			const float sphereRadius = 1.11f * (-light.linear + glm::sqrt(light.linear * light.linear - 4.f * light.quadratic * (light.constant - brightness * 256.f / minBrightness))) / (2.f * light.quadratic);

			pointShader->SetFloat("pointLight.radius", sphereRadius);

			glm::mat4 model = glm::mat4(1.f);

			model = glm::translate(model, pos);
			model = glm::scale(model, glm::vec3(sphereRadius));

			pointShader->SetMatrix("model", model);

			_lightSphereMesh->Draw(pointShader);
		});
		glCullFace(GL_BACK);
	}

	void TestState::Draw()
	{
		float pulseProgress = 0.75f + 0.25f * (float)sin(glfwGetTime());
		float constantProgress = (float)fmod(glfwGetTime(), 100.0) * 4.f;

		glm::mat4 view = GetCamera().GetViewMatrix();
		glm::mat4 proj = GetProjMatrix();

		ImGui::SliderFloat("Gamma", &_gamma, 1.f, 2.4f);
		ImGui::Checkbox("Blinn", &_enableBlinn);

		{
			glm::mat4 combined = proj * view;
			glm::mat4 combinedNoDir = proj * glm::mat4(glm::mat3(view));

			MyGL::UboManager::SetMatrix(MyGL::UboManager::BINDING_MATRICES, 0, proj);
			MyGL::UboManager::SetMatrix(MyGL::UboManager::BINDING_MATRICES, 1, view);
			MyGL::UboManager::SetMatrix(MyGL::UboManager::BINDING_MATRICES, 2, combined);

			MyGL::UboManager::SetMatrix(MyGL::UboManager::BINDING_MATRICES_EXT, 0, combinedNoDir);
			MyGL::UboManager::SetMatrix(MyGL::UboManager::BINDING_MATRICES_EXT, 1, glm::inverse(combined));
			MyGL::UboManager::SetMatrix(MyGL::UboManager::BINDING_MATRICES_EXT, 2, glm::inverse(combinedNoDir));

			MyGL::UboManager::SetVector(MyGL::UboManager::BINDING_VECTORS, 0, glm::vec4(GetCamera().GetPosition(), 0.f));

			MyGL::UboManager::SetScalar(MyGL::UboManager::BINDING_SCALARS, 0, _gamma);
		}

		Pass1();
		Pass2();

		// Forward
		DrawSkybox();

		_framebufferPass2->EndRender();


		// Apply gamma correction
		if (_gamma > 1.f) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			ShaderPtr gammaShader = ResourceManager::Instance()->GetShader("Gamma");
			gammaShader->Bind();
			gammaShader->SetFloat("gamma", _gamma);

			glActiveTexture(GL_TEXTURE0);
			_colorPass2->Bind();
			gammaShader->SetInt("texture1", 0);

			StateManager::Instance()->SetState(StateType::OpaqueSprite);
			DrawFullscreen(gammaShader);
		}
		else {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, _framebufferPass2->GetId());
			glBlitFramebuffer(0, 0, _vpWidth, _vpHeight, 0, 0, _vpWidth, _vpHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		ImGui::Checkbox("Debug g-buffer", &_debugGBuffer);
		if (_debugGBuffer) {
			StateManager::Instance()->SetState(StateType::OpaqueSprite);

			_deferredRenderer->DrawDebug();
		}
	}

	void TestState::MouseDown(int button, float x, float y)
	{
		auto sphereEntity = _scene->CreateEntity();
		_scene->AddComponent(sphereEntity, std::make_shared<TestComponent>());

		sphereEntity->SetMesh(_sphereMesh);
		//sphereEntity->SetMaterial(std::make_shared<CubemapMaterial>(ResourceManager::Instance()->GetCubemap("Skybox")));
		sphereEntity->SetMaterial(std::make_shared<VertexColorMaterial>());

		auto physicsComponent = std::make_shared<PhysicsComponent>(_sphereShape.get(), 1.f);
		_scene->AddComponent(sphereEntity, physicsComponent);

		auto lightComponent = std::make_shared<LightComponent>(glm::vec3(1.f, 1.f, 1.f), 1.f, 0.7f, 1.8f);
		_scene->AddComponent(sphereEntity, lightComponent);

		glm::vec3 dir;

		if (button == 1) {
			dir = CameraRay(x, y);
		} else if (button == 0) {
			dir = _camera->GetDirection();
		}

		sphereEntity->position = _camera->GetPosition() + dir * 2.f;
		physicsComponent->pendingImpulse = dir * 70.f;
	}

	void TestState::MouseUp(int button, float x, float y)
	{

	}

	void TestState::CreateFramebuffer()
	{
	}

	void TestState::DrawSkybox() {
		MyGL::CubemapPtr skyboxTexture = MyGL::ResourceManager::Instance()->GetCubemap("Skybox");
		MyGL::ShaderPtr shader = MyGL::ResourceManager::Instance()->GetShader("Skybox");

		StateManager::Instance()->SetState(StateType::Skybox);

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
	}

	void TestState::DrawFullscreen(ShaderPtr& shader)
	{
		shader->SetFloat("quadScale", 2.f);
		shader->SetFloat2("quadShift", -1.f, -1.f);

		glBindVertexArray(_emptyVao);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	glm::vec3 TestState::Gamma(const glm::vec3 & color)
	{
		return glm::pow(color, glm::vec3(_gamma));
	}

	glm::mat4 TestState::GetProjMatrix() const
	{
		return glm::perspective(glm::radians(45.f), (float)_vpWidth / _vpHeight, 1.f, 500.f);
	}

	glm::vec3 TestState::CameraRay(float x, float y) const
	{
		glm::vec4 clipPos(2.f * x / _vpWidth - 1.f, 1.f - 2.f * y / _vpHeight, 0.5f, 1.f);
		glm::mat4 unprojectMat = glm::inverse(GetProjMatrix() * glm::mat4(glm::mat3(GetCamera().GetViewMatrix())));
		glm::vec4 worldPos = unprojectMat * clipPos;
		worldPos /= worldPos.w;

		return glm::normalize(glm::vec3(worldPos));
	}
}
