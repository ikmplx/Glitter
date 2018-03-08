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

		_scene = std::make_shared<MyGL::Scene>();

		_centerEntity = _scene->CreateEntity();

		_nanosuitEntity1 = _nanosuitPrefab->Clone();
		_nanosuitEntity2 = _nanosuitPrefab->Clone();

		_nanosuitEntity1->position += glm::vec3(-10, 0, 0);
		_nanosuitEntity2->position += glm::vec3(10, 0, 0);

		_towerEntity = _towerPrefab->Clone();
		_towerEntity->position += glm::vec3(0, 0, -10);

		_floorEntity = _scene->CreateEntity();
		_floorEntity->SetMesh(Primitives::CreatePlane(100.f, 100.f, 0.1f));

		_centerEntity->AddChild(_nanosuitEntity1);
		_centerEntity->AddChild(_nanosuitEntity2);
		_centerEntity->AddChild(_towerEntity);
		_centerEntity->AddChild(_floorEntity);


		CreateFramebuffer();
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
		}


		_mainFramebuffer->BeginRender();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto shader = MyGL::ResourceManager::Instance()->GetShader("test");
		shader->Bind();

		_scene->Draw(shader);

		DrawSkybox();

		_mainFramebuffer->EndRender();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		glClearColor(1.f, 0.f, 1.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(_emptyVao);

		glActiveTexture(GL_TEXTURE0);
		_mainAttachment->Bind();

		ShaderPtr vpShader = ResourceManager::Instance()->GetShader("vpquad");
		vpShader->Bind();
		vpShader->SetInt("texture1", 0);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	void TestState::CreateFramebuffer()
	{
		_mainAttachment = std::make_shared<Attachment>(_vpWidth, _vpHeight, Attachment::Type::RGB, false);
		_mainFramebuffer = std::make_shared<Framebuffer>();

		_mainFramebuffer->AttachColor(_mainAttachment);
		_mainFramebuffer->AttachDepth(std::make_shared<Attachment>(_vpWidth, _vpHeight, Attachment::Type::Depth, false));
	}

	void TestState::DrawSkybox() {
		MyGL::CubemapPtr skyboxTexture = MyGL::ResourceManager::Instance()->GetCubemap("Skybox");
		MyGL::ShaderPtr shader = MyGL::ResourceManager::Instance()->GetShader("Skybox");

		//glCullFace(GL_FRONT);
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
		//glCullFace(GL_BACK);
	}
}
