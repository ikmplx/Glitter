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

namespace MyGL
{
	TestState::TestState()
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
	}



	TestState::~TestState()
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
		glm::mat4 proj = glm::perspective(glm::radians(45.f), (float)_vpWidth / _vpHeight, 1.f, 100.f);

		MyGL::UboManager::SetMatrices(0, proj);
		MyGL::UboManager::SetMatrices(1, view);
		MyGL::UboManager::SetMatrices(2, proj * view);

		auto shader = MyGL::ResourceManager::Instance()->GetShader("test");
		shader->Bind();

		_scene->Draw(shader);

		DrawSkybox();
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
