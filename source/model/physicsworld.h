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
		PhysicsWorld(std::shared_ptr<GameLogic>& gameLogic, bool useDebugView = false);
		virtual ~PhysicsWorld();

		void initializeWorld();
		void stepSimulation(const long double currentTime);

		void addRigidBodies(const std::vector<std::shared_ptr<btRigidBody>>& bodies, const short group = 0, const short mask = 0);
		void addRigidBody(btRigidBody *body, const short group = 0, const short mask = 0);
		void removeRigidBodies(const std::vector<std::shared_ptr<btRigidBody>>& bodies);
		void removeRigidBody(btRigidBody* body);
		void addCollisionObject(btCollisionObject* obj);
		void removeCollisionObject(btCollisionObject* obj);
		// debugview
		util::GLDebugDrawer* m_debug;

		
		btDiscreteDynamicsWorld* getDiscreteWorld()
		{
			return m_world;
		};

	private:
		btDiscreteDynamicsWorld*			m_world;
		btSequentialImpulseConstraintSolver*m_solver;
		btDefaultCollisionConfiguration*	m_collisionConfiguration;
		btCollisionDispatcher*				m_dispatcher;
		btBroadphaseInterface*				m_broadphase;

		// collision events
		void checkForCollisionEvents();
		CollisionPairSet m_pairsLastUpdate;

		// steping variables
		long double m_lastSimulationTime;

		bool m_useDebugView;
		std::weak_ptr<GameLogic> m_gameLogic;
	};
}