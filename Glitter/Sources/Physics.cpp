// ¯\_(ツ)_/¯

#include "stdafx.h"

#include "Physics.h"
#include "Scene/Entity.h"
#include "Utils.h"

namespace MyGL
{
	RigidBody::RigidBody(EntityPtr entity, btCollisionShape* collisionShape, float mass)
		: _rbInfo(mass, this, collisionShape)
		, _entity(entity)
	{
		if (mass != 0.f) {
			collisionShape->calculateLocalInertia(mass, _rbInfo.m_localInertia);
		}
	}

	RigidBody::~RigidBody()
	{
	}

	bool RigidBody::IsPhysicsTransform() const
	{
		return _rigidBody != nullptr && _isTransformInitialized;
	}
	
	void RigidBody::getWorldTransform(btTransform& worldTrans) const
	{
		EntityPtr entityHardPtr = _entity.lock();
		if (entityHardPtr) {
			worldTrans.setFromOpenGLMatrix(reinterpret_cast<const btScalar*>(&entityHardPtr->GetGlobalTransform()));
		}
	}
	
	void RigidBody::setWorldTransform(const btTransform& worldTrans)
	{
		EntityPtr entityHardPtr = _entity.lock();
		if (entityHardPtr) {
			worldTrans.getOpenGLMatrix(reinterpret_cast<btScalar*>(&entityHardPtr->_globalTransform));
		}

		_isTransformInitialized = true;
	}

	Physics* Physics::_sInstance;

	Physics::Physics()
	{
		_collisionConfiguration = new btDefaultCollisionConfiguration();
		_dispatcher = new btCollisionDispatcher(_collisionConfiguration);
		_broadphase = new btDbvtBroadphase();
		_solver = new btSequentialImpulseConstraintSolver();
		_world = new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _solver, _collisionConfiguration);

		//_world->setGravity(btVector3(0.f, -9.8f, 0.f));
	}

	Physics::~Physics()
	{
		delete _world;
		delete _solver;
		delete _broadphase;
		delete _dispatcher;
		delete _collisionConfiguration;
	}

	void Physics::Initialize()
	{
		_sInstance = new Physics();
	}

	void Physics::Deinitialize()
	{
		delete _sInstance;
	}

	Physics* Physics::Instance()
	{
		return _sInstance;
	}

	void Physics::Update(float dt)
	{
		for (auto& rigidBody : _addRigidBodies) {
			rigidBody->_rigidBody = new btRigidBody(rigidBody->_rbInfo);
			_world->addRigidBody(rigidBody->_rigidBody);

			auto emplaceResult = _rigidBodies.emplace(rigidBody->_rigidBody, rigidBody);

			MyAssert(emplaceResult.second);
		}
		_addRigidBodies.clear();

		_world->stepSimulation(dt, 2);
	}

	void Physics::AddRigidBody(RigidBodyPtr rigidBody)
	{
		_addRigidBodies.push_back(rigidBody);
	}
}
