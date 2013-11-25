#pragma once
// STD
#include <memory>
#include <vector>
// troen
#include "../forwarddeclarations.h"

namespace troen
{
	class PhysicsWorld
	{
	public:
		PhysicsWorld();

		virtual ~PhysicsWorld();

		void initializeWorld();
		void stepSimulation(long double currentTime);
		void checkForCollisions();

		void addRigidBodies(std::shared_ptr<std::vector<btRigidBody>> bodies);

		util::GLDebugDrawer* m_debug;

	private:
		// member variables
		btDiscreteDynamicsWorld *m_world;

		btSequentialImpulseConstraintSolver *m_solver;
		btDefaultCollisionConfiguration *m_collisionConfiguration;
		btCollisionDispatcher *m_dispatcher;
		btBroadphaseInterface *m_broadphase;

		long double m_lastSimulationTime;
	};
}