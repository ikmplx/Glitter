// ¯\_(ツ)_/¯

#include "stdafx.h"

#include "Physics.h"
#include "Scene/Entity.h"
#include "Utils.h"
#include "Scene/Component.h"

namespace MyGL
{
	PhysicsComponent::PhysicsComponent(btCollisionShape* collisionShape, float mass)
		: collisionShape(collisionShape)
	    , mass(mass)
	{
	}

	class PhysicsMotionState : public btMotionState
	{
	public:
		explicit PhysicsMotionState(EntityPtr entity)
			: _entity(std::move(entity))
		{
		}

		void getWorldTransform(btTransform& worldTrans) const override
		{
			worldTrans.setFromOpenGLMatrix(reinterpret_cast<const btScalar*>(&_entity->GetGlobalTransform()));
		}

		void setWorldTransform(const btTransform& worldTrans) override
		{
			worldTrans.getOpenGLMatrix(reinterpret_cast<btScalar*>(&_entity->externalTransform));
			_entity->hasExternalTransform = true;
			_entity->InvalidateTransform();
		}

	private:
		EntityPtr _entity;
	};

	PhysicsSystem::PhysicsSystem()
		: System(typeid(PhysicsComponent))
	{
		_collisionConfiguration = new btDefaultCollisionConfiguration();
		_dispatcher = new btCollisionDispatcher(_collisionConfiguration);
		_broadphase = new btDbvtBroadphase();
		_solver = new btSequentialImpulseConstraintSolver();
		_world = new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _solver, _collisionConfiguration);
	}

	PhysicsSystem::~PhysicsSystem()
	{
		delete _world;
		delete _solver;
		delete _broadphase;
		delete _dispatcher;
		delete _collisionConfiguration;
	}

	void PhysicsSystem::Update(ScenePtr scene, float dt)
	{
		for (auto& entity : GetEntities()) {
			auto comp = entity->FindComponent<PhysicsComponent>();

			if (!comp->rigidBody) {
				comp->motionState = std::make_unique<PhysicsMotionState>(entity);
				btRigidBody::btRigidBodyConstructionInfo rbInfo(comp->mass, comp->motionState.get(), comp->collisionShape);
				if (comp->mass != 0.f) {
					comp->collisionShape->calculateLocalInertia(comp->mass, rbInfo.m_localInertia);
				}
				
				comp->rigidBody  = std::make_unique<btRigidBody>(rbInfo);

				_world->addRigidBody(comp->rigidBody.get());
				_physicEntities[entity] = comp;
			}
		}

		_world->stepSimulation(dt, 2);
	}

	void PhysicsSystem::BeforeEntityRemove(EntityPtr entity)
	{
		auto iter = _physicEntities.find(entity);
		if (iter != _physicEntities.end()) {
			auto& comp = iter->second;
			_world->removeRigidBody(comp->rigidBody.get());
			comp->rigidBody = nullptr;
			comp->motionState = nullptr;

			_physicEntities.erase(iter);
		}
	}
}
