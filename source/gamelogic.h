#pragma once

#include "forwarddeclarations.h"
#include <memory>
#include <vector>

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

			void addRigidBodies(std::shared_ptr<std::vector<btRigidBody>> bodies);
		
			// TODO remove
			btRigidBody *m_fallingRigidBody;
			

		private:
			// member variables
			btDiscreteDynamicsWorld *m_world;

			btSequentialImpulseConstraintSolver *m_solver;
			btDefaultCollisionConfiguration *m_collisionConfiguration;
			btCollisionDispatcher *m_dispatcher;
			btBroadphaseInterface *m_broadphase;


			

	};
	
}