// ¯\_(ツ)_/¯

#pragma once

#include "Forward.h"
#include "Scene/System.h"
#include "Scene/Component.h"

namespace MyGL
{
	struct TriangleShapeHolder
	{
		std::vector<MeshPtr> sourceMeshes;
		std::unique_ptr<btTriangleIndexVertexArray> array;
		std::unique_ptr<btBvhTriangleMeshShape> shape;
	};
	using TriangleShapeHolderPtr = std::shared_ptr<TriangleShapeHolder>;

	class PhysicsComponent : public TypedComponent<PhysicsComponent>
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

		static TriangleShapeHolderPtr CreateTriangleShape(const EntityPtr& entity);

	private:
		btDefaultCollisionConfiguration* _collisionConfiguration = nullptr;
		btCollisionDispatcher* _dispatcher = nullptr;
		btBroadphaseInterface* _broadphase = nullptr;
		btSequentialImpulseConstraintSolver* _solver = nullptr;
		btDiscreteDynamicsWorld* _world = nullptr;

		std::vector<std::shared_ptr<PhysicsComponent>> _components;
	};
}
