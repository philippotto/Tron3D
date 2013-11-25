#include "PhysicsWorld.h"
// STD
#include <iostream>
// bullet
#include <btBulletDynamicsCommon.h>
#include "LinearMath/btHashMap.h"

//troen
#include "../util/gldebugdrawer.h"
#include "../controller/abstractcontroller.h"
#include "../controller/bikecontroller.h"
#include "../sound/audiomanager.h"

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

	m_world->setGravity(btVector3(0, 0, -100));
}

void PhysicsWorld::addRigidBodies(const std::vector<std::shared_ptr<btRigidBody>>& bodies)
{
	/*
	why does this lead to a crash ?
	for (btRigidBody body : *(bodies))
		m_world->addRigidBody(&body);
	
	the old fashioned way works: 	*/

	for (size_t i = 0; i < bodies.size(); i++)
		m_world->addRigidBody(bodies[i].get());
		
}

void PhysicsWorld::addRigidBody(btRigidBody *body) {
	m_world->addRigidBody(body);
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
	//m_world->stepSimulation(1 / 60.f, 10);
	
#if defined DEBUG
	m_world->debugDrawWorld();
	m_debug->EndDraw();
#endif

	checkForCollisionEvents();
}

void PhysicsWorld::checkForCollisionEvents() {
	// keep a list of the collision pairs we
	// found during the current update
	CollisionPairSet pairsThisUpdate;
	
	// iterate through all of the manifolds in the dispatcher
	for (int i = 0; i < m_dispatcher->getNumManifolds(); ++i) {
		
		// get the manifold
		btPersistentManifold* pManifold = m_dispatcher->getManifoldByIndexInternal(i);

		// ignore manifolds that have 
		// no contact points.
		if (pManifold->getNumContacts() > 0) {
			// get the two rigid bodies involved in the collision
			const btRigidBody* pBody0 = static_cast<const btRigidBody*>(pManifold->getBody0());
			const btRigidBody* pBody1 = static_cast<const btRigidBody*>(pManifold->getBody1());
			
			// always create the pair in a predictable order
			// (use the pointer value..)
			bool const swapped = pBody0 > pBody1;
			const btRigidBody* pSortedBodyA = swapped ? pBody1 : pBody0;
			const btRigidBody* pSortedBodyB = swapped ? pBody0 : pBody1;
			
			// create the pair
			CollisionPair thisPair = std::make_pair(pSortedBodyA, pSortedBodyB);
			
			// insert the pair into the current list
			pairsThisUpdate.insert(thisPair);
			
			// if this pair doesn't exist in the list
			// from the previous update, it is a new
			// pair and we must send a collision event
			if (m_pairsLastUpdate.find(thisPair) == m_pairsLastUpdate.end()) {
					collisionEvent((btRigidBody*)pBody0, (btRigidBody*)pBody1);			
			}	
		}	
	}
	
	// create another list for pairs that
	// were removed this update
	CollisionPairSet removedPairs;
	
	// this handy function gets the difference beween
	// two sets. It takes the difference between
	// collision pairs from the last update, and this 
	// update and pushes them into the removed pairs list
	std::set_difference(m_pairsLastUpdate.begin(), m_pairsLastUpdate.end(),
		pairsThisUpdate.begin(), pairsThisUpdate.end(),
		std::inserter(removedPairs, removedPairs.begin()));
	
	// iterate through all of the removed pairs
	// sending separation events for them
	for (CollisionPairSet::const_iterator iter = removedPairs.begin(); iter != removedPairs.end(); ++iter)
	{
			separationEvent((btRigidBody*)iter->first, (btRigidBody*)iter->second);	
	}
	
	// in the next iteration we'll want to
	// compare against the pairs we found
	// in this iteration
	m_pairsLastUpdate = pairsThisUpdate;
}

void PhysicsWorld::collisionEvent(btRigidBody * pBody0, btRigidBody * pBody1) {
	// find the two colliding objects
	// std::shared_ptr<AbstractController> pObj0 = findGameObject(pBody0);
	// std::shared_ptr<AbstractController> pObj1 = findGameObject(pBody1);
	
	AbstractController * pObj0 = static_cast<AbstractController *>(pBody0->getUserPointer());
	AbstractController * pObj1 = static_cast<AbstractController *>(pBody1->getUserPointer());;

	//std::cout << "[PhysicsWorld::collisionEvent] collision detected" << std::endl;
	
	// exit if we didn't find anything
	if (!pObj0 && !pObj1) return;
	
	if (pObj0 && pObj0->getType() == AbstractController::BIKECONTROLLER)
	{
		std::cout << "BikeCollision, bike as first object" << std::endl;
		((BikeController*)(pObj0) )->getAudioManager()->PlaySFX("data/sound/explosion.wav",1,1,1,1);
	}
	else if (pObj1 && pObj1->getType() == AbstractController::BIKECONTROLLER)
	{
		std::cout << "BikeCollision, bike as second object" << std::endl;
	}
	// handel colision events object specific
	// set their colors to white
	// pObj0->SetColor(btVector3(1.0, 1.0, 1.0));
	// pObj1->SetColor(btVector3(1.0, 1.0, 1.0));
}

void PhysicsWorld::separationEvent(btRigidBody * pBody0, btRigidBody * pBody1) {
	// get the two separating objects
	std::shared_ptr<AbstractController> pObj0 = findGameObject((btRigidBody*)pBody0);
	std::shared_ptr<AbstractController> pObj1 = findGameObject((btRigidBody*)pBody1);
	
	//std::cout << "[PhysicsWorld::seperationEvent] seperation detected" << std::endl;

	// exit if we didn't find anything
	if (!pObj0 || !pObj1) return;
	
	// TODO: handle seperation events object specific
	//// set their colors to black
	//pObj0->SetColor(btVector3(0.0, 0.0, 0.0));
	//pObj1->SetColor(btVector3(0.0, 0.0, 0.0));
}

std::shared_ptr<AbstractController> PhysicsWorld::findGameObject(btRigidBody* pBody) {
	//// search through our list of gameobjects finding
	//// the one with a rigid body that matches the given one
	//for (GameObjects::iterator iter = m_objects.begin(); iter != m_objects.end(); ++iter) {
	//		if ((*iter)->GetRigidBody() == pBody) {
	//				// found the body, so return the corresponding game object
	//				return *iter;
	//	}
	//	
	//}
	return 0;
}