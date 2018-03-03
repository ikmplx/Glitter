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

		_skyboxCube = Primitives::CreateCube();
		_skyboxCube->material = nullptr;

		CreateFramebuffer();
	}

	void TestState::Deinit()
	{
		glDeleteFramebuffers(1, &_fbo);
	}

	void TestState::Update(float dt)
	{
	}

	void TestState::Draw()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float pulseProgress = 0.75f + 0.25f * (float)sin(glfwGetTime());
		float constantProgress = (float)fmod(glfwGetTime(), 100.0) * 4.f;

		glm::mat4 view = GetCamera().GetViewMatrix();
		glm::mat4 proj = glm::perspective(glm::radians(45.f), (float)_vpWidth / _vpHeight, 1.f, 100.f);

		MyGL::UboManager::SetMatrices(0, proj);
		MyGL::UboManager::SetMatrices(1, view);
		MyGL::UboManager::SetMatrices(2, proj * view);

		auto shader = MyGL::ResourceManager::Instance()->GetShader("test");
		shader->Bind();

		_scene->Draw(shader);

		DrawSkybox();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, _vpWidth, _vpHeight);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		glClearColor(1.f, 0.f, 1.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(_vpVao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _fboTexAtt);

		ShaderPtr vpShader = ResourceManager::Instance()->GetShader("vpquad");
		vpShader->Bind();
		vpShader->SetInt("texture1", 0);

		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);
	}

	void TestState::CreateFramebuffer()
	{
		// Framebuffer
		int fbWidth = _vpWidth;
		int fbHeight = _vpHeight;

		glGenFramebuffers(1, &_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

		glGenTextures(1, &_fboTexAtt);
		glBindTexture(GL_TEXTURE_2D, _fboTexAtt);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fbWidth, fbHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _fboTexAtt, 0);

		GLuint depthAtt;
		glGenTextures(1, &depthAtt);
		glBindTexture(GL_TEXTURE_2D, depthAtt);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, fbWidth, fbHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthAtt, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Viewport VAO
		glGenVertexArrays(1, &_vpVao);
	}

	void TestState::DrawSkybox() {
		MyGL::CubemapPtr skyboxTexture = MyGL::ResourceManager::Instance()->GetCubemap("Skybox");
		MyGL::ShaderPtr shader = MyGL::ResourceManager::Instance()->GetShader("Skybox");

		glCullFace(GL_FRONT);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);

		glActiveTexture(GL_TEXTURE0);
		skyboxTexture->Bind();

		shader->Bind();
		shader->SetInt("sampler", 0);

		_skyboxCube->Draw();

		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);
		glCullFace(GL_BACK);
	}
}
