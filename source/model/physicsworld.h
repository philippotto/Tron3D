#pragma once
// STD
#include <vector>
#include <set>
#include <iterator>
#include <algorithm>
// troen
#include "../forwarddeclarations.h"

// typedefs for collision events
typedef std::vector<std::shared_ptr<troen::AbstractController>> ControllerVector;
typedef std::pair<const btRigidBody*, const btRigidBody*> CollisionPair;
typedef std::set<CollisionPair> CollisionPairSet;

namespace troen
{
	class PhysicsWorld
	{
	public:
		PhysicsWorld();
		virtual ~PhysicsWorld();

		void initializeWorld();
		void stepSimulation(long double currentTime);

		void addRigidBodies(const std::vector<std::shared_ptr<btRigidBody>>& bodies);
		void addRigidBody(btRigidBody* body);
		void removeRigidBodies(const std::vector<std::shared_ptr<btRigidBody>>& bodies);
		void removeRigidBody(btRigidBody* body);

		// collision event functions
		void checkForCollisionEvents();
		virtual void collisionEvent(btRigidBody* pBody0, btRigidBody * pBody1);
		virtual void separationEvent(btRigidBody * pBody0, btRigidBody * pBody1);

		// debugview
		util::GLDebugDrawer* m_debug;

	private:
		btDiscreteDynamicsWorld *m_world;
		btSequentialImpulseConstraintSolver *m_solver;
		btDefaultCollisionConfiguration *m_collisionConfiguration;
		btCollisionDispatcher *m_dispatcher;
		btBroadphaseInterface *m_broadphase;

		// collision event variables
		CollisionPairSet m_pairsLastUpdate;

		// steping variables
		long double m_lastSimulationTime;
	};
}