// ¯\_(ツ)_/¯
#include "stdafx.h"

#include "Entity.h"
#include "Res/Shader.h"
#include "Model/Mesh.h"
#include "Component.h"

namespace MyGL
{
	Entity::Entity()
		: position(glm::vec3(0))
		, scale(glm::vec3(1))
		, rotation(glm::angleAxis(0.f, glm::vec3(0, 0, 1)))
	{
	}

	Entity::~Entity() = default;

	void Entity::CalcGlobalTransform()
	{
		glm::mat4 identity(1);

		EntityPtr parentEntity = _parent.lock();
		if (parentEntity) {
			_globalScale = parentEntity->_globalScale * scale;
			_globalRotation = parentEntity->_globalRotation * rotation;
			_globalPosition = parentEntity->_globalPosition + parentEntity->rotation * position * parentEntity->scale;
		}
		else {
			_globalRotation = rotation;
			_globalScale = scale;
			_globalPosition = position;
		}

		_globalTransform = glm::translate(identity, _globalPosition) * glm::mat4_cast(_globalRotation) * glm::scale(identity, _globalScale);
	}

	EntityPtr Entity::Clone()
	{
		EntityPtr cloneRoot = std::make_shared<Entity>();

		cloneRoot->position = position;
		cloneRoot->rotation = rotation;
		cloneRoot->scale = scale;
		cloneRoot->_mesh = _mesh;

		for (auto& child : _children) {
			EntityPtr cloneChild = child->Clone();

			cloneChild->_parent = cloneRoot;
			cloneRoot->_children.push_back(cloneChild);
		}

		return cloneRoot;
	}

	EntityPtr Entity::CreateChild()
	{
		EntityPtr entity = std::make_shared<Entity>();
		AddChild(entity);
		return entity;
	}

	void Entity::AddChild(EntityPtr child)
	{
		_children.push_back(child);
		child->_parent = weak_from_this();
	}

	void Entity::SetMesh(MeshPtr mesh)
	{
		_mesh = mesh;
	}

	void Entity::Draw(ShaderPtr shader)
	{
		if (_mesh) {
			shader->SetMatrix("model", _globalTransform);
			_mesh->Draw(shader);
		}
	}

	void Entity::AddComponent(ComponentPtr component)
	{
		_components.push_back(component);
		component->entity = weak_from_this();
	}

	ComponentPtr Entity::FindComponent(const std::type_info & typeInfo)
	{
		for (const auto& component : _components) {
			if (typeid(*component) == typeInfo) {
				return component;
			}
		}

		return nullptr;
	}
}