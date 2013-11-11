#pragma once

// TODO
// jd: why do i need the " ../" , really shouldn't be necessary
#include "../forwarddeclarations.h"


namespace troen
{
	namespace physics
	{
		class PhysicsWorld
		{
			public:
				PhysicsWorld();

				virtual ~PhysicsWorld();

				void initialize();
				void initializeWorld();

				void createLevel();

				void stepSimulation();
				void addFence();
				void addBike();
				void addItemBox();
				void checkForCollisions();

				// member variables
				btDiscreteDynamicsWorld *m_world;

				btSequentialImpulseConstraintSolver *m_solver;
				btDefaultCollisionConfiguration *m_collisionConfiguration;
				btCollisionDispatcher *m_dispatcher;
				btBroadphaseInterface *m_broadphase;

				btRigidBody *m_fallingRigidBody;
				btRigidBody *m_groundRigidBody;

				btCollisionShape *m_fallingShape;
				btCollisionShape *m_groundShape;

		};
	}
}