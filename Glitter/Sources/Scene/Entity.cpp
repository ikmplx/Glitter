// ¯\_(ツ)_/¯
#include "stdafx.h"

#include "Entity.h"
#include "Res/Shader.h"
#include "Model/Mesh.h"
#include "Component.h"
#include "Physics.h"
#include "Model/Material.h"

namespace MyGL
{
	Entity::Entity()
		: _material(new Material())
	{
	}

	Entity::~Entity() = default;

	const glm::mat4& Entity::GetGlobalTransform() const
	{
		if (_isGlobalTransformNeedUpdate) {
			EntityPtr parentEntity = _parent.lock();
			if (parentEntity) {
				_globalTransform = parentEntity->GetGlobalTransform();
			} else {
				_globalTransform = glm::mat4(1.f);
			}

			if (hasExternalTransform) {
				// TODO: should manage scale with PhysicsSystem (or use child node for scaling mesh)
				_globalTransform *= externalTransform;
			}
			else {
				_globalTransform = glm::translate(_globalTransform, position);
				_globalTransform *= glm::mat4_cast(rotation);
				_globalTransform = glm::scale(_globalTransform, scale);
			}
			_isGlobalTransformNeedUpdate = false;
		}

		return _globalTransform;
	}

	glm::vec3 Entity::GetGlobalPosition() const
	{
		return glm::vec3(GetGlobalTransform()[3]);
	}

	EntityPtr Entity::Clone()
	{
		EntityPtr cloneRoot = std::make_shared<Entity>();

		cloneRoot->position = position;
		cloneRoot->rotation = rotation;
		cloneRoot->scale = scale;
		cloneRoot->_mesh = _mesh;
		cloneRoot->_material = _material;

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

	void Entity::SetMaterial(MaterialPtr material)
	{
		_material = material;
	}

	MaterialPtr Entity::GetMaterial()
	{
		return _material;
	}

	void Entity::Draw(ShaderPtr shader)
	{
		if (_mesh) {
			shader->Bind();
			shader->SetMatrix("model", GetGlobalTransform());
			_material->Bind(shader);
			_mesh->Draw();
		}
	}

	void Entity::InvalidateTransform()
	{
		_isGlobalTransformNeedUpdate = true;
		for (auto& child : _children) {
			child->InvalidateTransform();
		}
	}

	ComponentPtr Entity::FindComponent(const std::type_info & typeInfo)
	{
		for (const auto& component : _components) {
			auto& ref = *component.get();
			if (typeid(ref) == typeInfo) {
				return component;
			}
		}

		return nullptr;
	}
}