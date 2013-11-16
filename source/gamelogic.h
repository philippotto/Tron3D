#pragma once
// STD
#include <memory>
#include <vector>
// troen
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

			void addRigidBodies(std::shared_ptr<std::vector<btRigidBody>> bodies);
					

		private:
			// member variables
			btDiscreteDynamicsWorld *m_world;

			btSequentialImpulseConstraintSolver *m_solver;
			btDefaultCollisionConfiguration *m_collisionConfiguration;
			btCollisionDispatcher *m_dispatcher;
			btBroadphaseInterface *m_broadphase;


			

	};
	
}