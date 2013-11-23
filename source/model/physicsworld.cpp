#include "PhysicsWorld.h"
// STD
#include <iostream>
// bullet
#include <btBulletDynamicsCommon.h>
#include "LinearMath/btHashMap.h"

#include "../util/gldebugdrawer.h"


using namespace troen;

PhysicsWorld::PhysicsWorld() : m_lastSimulationTime(0)
{
	initializeWorld();

#if defined DEBUG
	m_debug = new util::GLDebugDrawer();
	m_debug->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	m_world->setDebugDrawer(m_debug);
#endif
}

PhysicsWorld::~PhysicsWorld()
{
	delete m_world;
	delete m_solver;
	delete m_collisionConfiguration;
	delete m_dispatcher;
	delete m_broadphase;
}

void PhysicsWorld::initializeWorld()
{
	// can be used to used to filter manually potential collision partners
	m_broadphase = new btDbvtBroadphase();

	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

	// potential bottleneck
	m_solver = new btSequentialImpulseConstraintSolver;

	m_world = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);

	m_world->setGravity(btVector3(0, 0, -10));
}

void PhysicsWorld::addRigidBodies(std::shared_ptr<std::vector<btRigidBody>> bodies)
{

	/*
	why does this lead to a crash ?
	for (btRigidBody body : *(bodies))
		m_world->addRigidBody(&body);
	
	the old fashioned way works: 	*/

	for (size_t i = 0; i < bodies->size(); i++)
		m_world->addRigidBody(&(bodies->at(i)));
		
}

void PhysicsWorld::stepSimulation(long double currentTime)
{	
	float timeSinceLastSimulation = currentTime - m_lastSimulationTime;
	m_lastSimulationTime = currentTime;

	// careful:
	// including this debug-printout will significantly reduce
	// framerate & flow of the game
	//std::cout << "[PhysicsWorld::stepSimulation] timeSinceLastSimulation = " << timeSinceLastSimulation/1000 << std::endl;
	
#if defined DEBUG
	m_debug->BeginDraw();
#endif
	
	// mind the following constraint:
	// timeStep < maxSubSteps * fixedTimeStep
	// where the parameters are given as follows:
	// stepSimulation(timeStep, maxSubSteps, fixedTimeStep)
	m_world->stepSimulation(timeSinceLastSimulation/1000.f, 7);
	
#if defined DEBUG
	m_world->debugDrawWorld();
	m_debug->EndDraw();
#endif

	checkForCollisions();

}

void PhysicsWorld::checkForCollisions()
{
	btDispatcher *dispatcher = m_world->getDispatcher();
	int numManifolds = dispatcher->getNumManifolds();
	

	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold =  dispatcher->getManifoldByIndexInternal(i);

		// unnecessary (?) and not working
		// btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
		// btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());

		int numContacts = contactManifold->getNumContacts();
		
		for (int j=0; j < numContacts; j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			
			
			// access our logic representation
			// contactManifold->getBody0()->getUserPointer();


			if (pt.getDistance() < 0.f)
			{
				// probably, we should "break" here for performance

				// additional infos
				/*const btVector3& ptA = pt.getPositionWorldOnA();
				const btVector3& ptB = pt.getPositionWorldOnB();
				const btVector3& normalOnB = pt.m_normalWorldOnB;*/
				
				// std::cout << "[PhysicsWorld::checkForCollisions] collision detected" << std::endl;
			}
		}
	}
}