#pragma once

#include "forwarddeclarations.h"


namespace troen
{

	class GameLogic
	{
		public:
			GameLogic();

			virtual ~GameLogic();

			void initialize();
			void initializeWorld();

			void createLevel();

			void stepSimulation();
			void addFence();
			void addBike();
			void addItemBox();
			void checkForCollisions();
		
		private:
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