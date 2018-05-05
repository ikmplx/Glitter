// ¯\_(ツ)_/¯

#pragma once

namespace MyGL
{
	class Physics
	{
	public:
		Physics();
		~Physics();

	private:
		btDefaultCollisionConfiguration* _collisionConfiguration;
		btCollisionDispatcher* _dispatcher;
		btBroadphaseInterface* _broadphase;
		btSequentialImpulseConstraintSolver* _solver;
		btDiscreteDynamicsWorld* _world;
	};
}
