// ¯\_(ツ)_/¯

#pragma once

#include "Forward.h"

namespace MyGL
{
	class Physics;

	class RigidBody : private btMotionState
	{
		friend class Physics;
	public:
		RigidBody(EntityPtr entity, btCollisionShape* collisionShape, float mass);
		~RigidBody();

		bool IsPhysicsTransform() const;

	private:
		virtual void getWorldTransform(btTransform& worldTrans) const override;
		virtual void setWorldTransform(const btTransform& worldTrans) override;

	private:
		EntityWeakPtr _entity;

		btRigidBody::btRigidBodyConstructionInfo _rbInfo;
		btRigidBody* _rigidBody = nullptr;
		bool _isTransformInitialized = false;
	};

	class Physics
	{
	public:
		Physics();
		~Physics();

		static void Initialize();
		static void Deinitialize();
		static Physics* Instance();

		void Update(float dt);

		void AddRigidBody(RigidBodyPtr rigidBody);

	private:
		static Physics* _sInstance;

		btDefaultCollisionConfiguration* _collisionConfiguration;
		btCollisionDispatcher* _dispatcher;
		btBroadphaseInterface* _broadphase;
		btSequentialImpulseConstraintSolver* _solver;
		btDiscreteDynamicsWorld* _world;
	
		std::vector<RigidBodyPtr> _addRigidBodies;
		std::unordered_map<btRigidBody*, RigidBodyPtr> _rigidBodies;
	};
}
