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

#include "Utils.h"

namespace MyGL
{
	TestState::TestState()
	{
	}



	TestState::~TestState()
	{
	}

	void TestState::Init()
	{
		GetCamera().SetPosition(glm::vec3(0.f, 7.f, 37.f));

		_nanosuitPrefab = ModelLoader::LoadModel("Models/nanosuit/nanosuit.obj");

		_towerPrefab = ModelLoader::LoadModel("Models/vox/chr_sword.ply");
		_towerPrefab->rotation = glm::angleAxis(glm::radians(-90.f), glm::vec3(1, 0, 0));
		_towerPrefab->Traverse([](Entity& entity) {
			if (entity.GetMesh()) {
				entity.GetMesh()->material->specularBase = 0.5f;
			}
		});

		_scene = std::make_shared<MyGL::Scene>();

		_centerEntity = _scene->CreateEntity();

		_nanosuitEntity1 = _nanosuitPrefab->Clone();
		_nanosuitEntity2 = _nanosuitPrefab->Clone();

		_nanosuitEntity1->position += glm::vec3(-10, 0, 0);
		_nanosuitEntity2->position += glm::vec3(10, 0, 0);

		_nanosuitEntity1->rotation = glm::angleAxis(glm::radians(35.f), glm::vec3(0, 1, 0));
		_nanosuitEntity2->rotation = glm::angleAxis(glm::radians(-35.f), glm::vec3(0, 1, 0));

		// _nanosuitEntity2->scale = glm::vec3(1.f, 5.f, 1.f);

		_towerEntity = _towerPrefab->Clone();
		_towerEntity->position += glm::vec3(0, 0, -10);

		_floorEntity = _scene->CreateEntity();
		auto floorPlane = Primitives::CreatePlane(100.f, 100.f, 0.1f);
		floorPlane->material->specularBase = 0.3f;
		_floorEntity->SetMesh(floorPlane);

		_centerEntity->AddChild(_nanosuitEntity1);
		_centerEntity->AddChild(_nanosuitEntity2);
		_centerEntity->AddChild(_towerEntity);
		_centerEntity->AddChild(_floorEntity);

		_deferredRenderer = std::make_shared<DeferredRenderer>(_vpWidth, _vpHeight);
	}

	void TestState::Deinit()
	{
	}

	void TestState::Update(float dt)
	{
	}

	void TestState::Draw()
	{
		float pulseProgress = 0.75f + 0.25f * (float)sin(glfwGetTime());
		float constantProgress = (float)fmod(glfwGetTime(), 100.0) * 4.f;

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

		_deferredRenderer->BeginPass1();

		auto shader = MyGL::ResourceManager::Instance()->GetShader("Pass1");
		shader->Bind();

		_scene->Draw(shader);

		_deferredRenderer->EndPass1();


		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		glClearColor(0.f, 0.f, 0.f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		// Ambient
		ShaderPtr ambientShader = ResourceManager::Instance()->GetShader("Pass2Ambient");
		ambientShader->Bind();
		ambientShader->SetVec3("ambientLight.color", glm::vec3(0.15f));
		_deferredRenderer->BindColorAttachments(ambientShader);
		DrawFullscreen();

		// Directional
		ShaderPtr dirShader = ResourceManager::Instance()->GetShader("Pass2Directional");
		dirShader->Bind();

		static glm::vec3 lightDir(0.2f, -1.f, -1.f);

		ImGui::SliderFloat3("Light", &lightDir.r, -1.f, 1.f);

		dirShader->SetVec3("dirLight.color", glm::vec3(0.5f, 0.65f, 0.5f));
		dirShader->SetVec3("dirLight.direction", lightDir);
		dirShader->SetFloat("dirLight.ambient", 0.f);

		_deferredRenderer->BindColorAttachments(dirShader);
		DrawFullscreen();

		glDisable(GL_BLEND);

		// Forward
		_deferredRenderer->GetFramebufferPass1()->BlitDepthToMainFramebuffer(_vpWidth, _vpHeight);
		DrawSkybox();
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

		shader->Bind();
		shader->SetInt("sampler", 0);

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
}
