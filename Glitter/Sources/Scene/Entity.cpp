﻿// ¯\_(ツ)_/¯
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

	void Entity::UpdateGlobalTransform() const
	{
		if (!_isGlobalTransformNeedUpdate) {
			return;
		}

		_globalTransform = glm::mat4(1.f);

		EntityPtr parentEntity = _parent.lock();
		if (parentEntity) {
			parentEntity->UpdateGlobalTransform();
			_globalTransform = parentEntity->_globalTransform;
		}

		_globalTransform = glm::translate(_globalTransform, position);
		_globalTransform *= glm::mat4_cast(rotation);
		_globalTransform = glm::scale(_globalTransform, scale);

		_isGlobalTransformNeedUpdate = false;
	}

	const glm::mat4& Entity::GetGlobalTransform() const
	{
		UpdateGlobalTransform();
		return _globalTransform;
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

	MeshPtr Entity::GetMesh()
	{
		return _mesh;
	}

	void Entity::Draw(ShaderPtr shader)
	{
		if (_mesh) {
			shader->SetMatrix("model", GetGlobalTransform());
			_mesh->Draw(shader);
		}
	}

	void Entity::InvalidateTransform()
	{
		_isGlobalTransformNeedUpdate = true;
		for (auto& child : _children) {
			child->InvalidateTransform();
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