// ¯\_(ツ)_/¯

#include "stdafx.h"

#include "Scene.h"
#include "Entity.h"
#include "Model/Mesh.h"
#include "Res/Shader.h"

namespace MyGL
{
	Scene::Scene()
		: _rootEntity(new Entity())
	{
	}

	Scene::~Scene() = default;

	EntityPtr Scene::CreateEntity()
	{
		return _rootEntity->CreateChild();
	}

	void Scene::AddEntity(EntityPtr ptr)
	{
		_rootEntity->AddChild(ptr);
	}

	void Scene::Draw(ShaderPtr shader)
	{
		_rootEntity->Traverse([&shader](Entity& entity) {
			entity.CalcGlobalTransform();
			entity.Draw(shader);
		});
	}
}

