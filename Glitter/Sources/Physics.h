// ¯\_(ツ)_/¯

#pragma once

#include "Forward.h"
#include "Scene/System.h"
#include "Scene/Component.h"

namespace MyGL
{
	class PhysicsComponent : public Component
	{
	public:
		PhysicsComponent(btCollisionShape* collisionShape, float mass);

		btCollisionShape* collisionShape;
		float mass = 0.f;

		glm::vec3 pendingImpulse = glm::vec3(0, 0, 0);

		std::unique_ptr<btRigidBody> rigidBody;
		std::unique_ptr<btMotionState> motionState;
	};

	class PhysicsSystem : public System
	{
	public:
		PhysicsSystem();
		~PhysicsSystem();

		void Update(ScenePtr scene, float dt) override;
		void BeforeEntityRemove(EntityPtr entity) override;

	private:
		btDefaultCollisionConfiguration* _collisionConfiguration = nullptr;
		btCollisionDispatcher* _dispatcher = nullptr;
		btBroadphaseInterface* _broadphase = nullptr;
		btSequentialImpulseConstraintSolver* _solver = nullptr;
		btDiscreteDynamicsWorld* _world = nullptr;

		std::unordered_map<EntityPtr, std::shared_ptr<PhysicsComponent>> _physicEntities;
	};
}
