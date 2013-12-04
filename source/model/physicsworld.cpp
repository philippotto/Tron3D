#include "PhysicsWorld.h"
// STD
#include <array>
// bullet
#include <btBulletDynamicsCommon.h>
#include "LinearMath/btHashMap.h"

//troen
#include "../util/gldebugdrawer.h"
#include "../model/abstractmodel.h"
#include "../controller/bikecontroller.h"
#include "../sound/audiomanager.h"

// comment out to disable debug mode
#define DEBUG_DRAW

using namespace troen;

PhysicsWorld::PhysicsWorld(std::shared_ptr<sound::AudioManager>& audioManager) :
m_lastSimulationTime(0), m_audioManager(audioManager)
{
	initializeWorld();

#if defined DEBUG_DRAW
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
	for (auto body : bodies)
		m_world->addRigidBody(body.get());
}

void PhysicsWorld::addRigidBody(btRigidBody *body) {
	m_world->addRigidBody(body);
}

void PhysicsWorld::removeRigidBodies(const std::vector<std::shared_ptr<btRigidBody>>& bodies)
{
	for (auto body : bodies)
		m_world->removeRigidBody(body.get());
}

void PhysicsWorld::removeRigidBody(btRigidBody* body)
{
	m_world->removeRigidBody(body);
}

void PhysicsWorld::stepSimulation(long double currentTime)
{	
	float timeSinceLastSimulation = currentTime - m_lastSimulationTime;
	m_lastSimulationTime = currentTime;

	// careful:
	// including this debug-printout will significantly reduce
	// framerate & flow of the game
	//std::cout << "[PhysicsWorld::stepSimulation] timeSinceLastSimulation = " << timeSinceLastSimulation/1000 << std::endl;
	
#if defined DEBUG_DRAW
	m_debug->BeginDraw();
#endif
	
	// mind the following constraint:
	// timeStep < maxSubSteps * fixedTimeStep
	// where the parameters are given as follows:
	// stepSimulation(timeStep, maxSubSteps, fixedTimeStep)
	m_world->stepSimulation(timeSinceLastSimulation/1000.f, 8);
	//m_world->stepSimulation(1 / 60.f, 10);
	
#if defined DEBUG_DRAW
	m_world->debugDrawWorld();
	m_debug->EndDraw();
#endif

	checkForCollisionEvents();
}

void PhysicsWorld::checkForCollisionEvents()
{
	// keep a list of the collision pairs we
	// found during the current update
	CollisionPairSet pairsThisUpdate;
	
	// iterate through all of the manifolds in the dispatcher
	for (int i = 0; i < m_dispatcher->getNumManifolds(); ++i) {
		
		// get the manifold
		btPersistentManifold* pManifold = m_dispatcher->getManifoldByIndexInternal(i);

		// ignore manifolds that have no contact points.
		if (pManifold->getNumContacts() > 0)
		{
			// get the two rigid bodies involved in the collision
			const btRigidBody* pBody0 = static_cast<const btRigidBody*>(pManifold->getBody0());
			const btRigidBody* pBody1 = static_cast<const btRigidBody*>(pManifold->getBody1());
			
			// create the pair in a predictable order (using the pointer value)
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
			if (m_pairsLastUpdate.find(thisPair) == m_pairsLastUpdate.end())
					collisionEvent((btRigidBody*)pBody0, (btRigidBody*)pBody1);			
		}	
	}
	
	// create another list for pairs that were removed this update
	CollisionPairSet removedPairs;
	
	// get the difference between collision pairs from the last update,
	// and this update and pushes them into the removed pairs list
	std::set_difference(m_pairsLastUpdate.begin(), m_pairsLastUpdate.end(),
		pairsThisUpdate.begin(), pairsThisUpdate.end(),
		std::inserter(removedPairs, removedPairs.begin()));
	
	// iterate through all of the removed pairs sending separation events for them
	for (CollisionPairSet::const_iterator iter = removedPairs.begin(); iter != removedPairs.end(); ++iter)
			separationEvent((btRigidBody*)iter->first, (btRigidBody*)iter->second);
	
	// in the next iteration we'll want to compare against
	// the pairs we found in this iteration
	m_pairsLastUpdate = pairsThisUpdate;
}

void PhysicsWorld::collisionEvent(btRigidBody * pBody0, btRigidBody * pBody1)
{
	//std::cout << "[PhysicsWorld::collisionEvent] collision detected" << std::endl;
	btRigidBody * collidingBodies[2];
	collidingBodies[0] = pBody0;
	collidingBodies[1] = pBody1;

	// get the controllers of the colliding objects
	AbstractController* collisionBodyControllers[2];
	collisionBodyControllers[0] = static_cast<AbstractController *>(pBody0->getUserPointer());
	collisionBodyControllers[1] = static_cast<AbstractController *>(pBody1->getUserPointer());
	
	// exit either controlles was not found
	if (!collisionBodyControllers[0] || !collisionBodyControllers[1]) return;

	std::array<COLLISIONTYPE,2> collisionTypes;
	collisionTypes[0] = static_cast<COLLISIONTYPE>(collidingBodies[0]->getUserIndex());
	collisionTypes[1] = static_cast<COLLISIONTYPE>(collidingBodies[1]->getUserIndex());
	
	// handle colision events object specific
	auto bikeIterator = std::find(collisionTypes.cbegin(), collisionTypes.cend(), BIKETYPE);
	if (bikeIterator != collisionTypes.cend())
	{
		int bikeIndex = bikeIterator - collisionTypes.cbegin();
		int otherIndex = bikeIndex == 0 ? 1 : 0;
		switch (collisionTypes[otherIndex])
		{
		case FENCETYPE:
		case LEVELTYPE:
		case LEVELWALLTYPE:
		case BIKETYPE:
			m_audioManager.lock()->PlaySFX("data/sound/explosion.wav", .5f, 1.f, .5f, 1.f);
			break;
		case LEVELGROUNDTYPE:

		default:
			break;
		}
	}
}

void PhysicsWorld::separationEvent(btRigidBody * pBody0, btRigidBody * pBody1)
{
	//std::cout << "[PhysicsWorld::seperationEvent] seperation detected" << std::endl;
	btRigidBody * collidingBodies[2];
	collidingBodies[0] = pBody0;
	collidingBodies[1] = pBody1;

	// get the controllers of the separating objects
	AbstractController* collisionBodyControllers[2];
	collisionBodyControllers[0] = static_cast<AbstractController *>(pBody0->getUserPointer());
	collisionBodyControllers[1] = static_cast<AbstractController *>(pBody1->getUserPointer());

	// exit either controlles was not found
	if (!collisionBodyControllers[0] || !collisionBodyControllers[1]) return;

	std::array<COLLISIONTYPE, 2> collisionTypes;
	collisionTypes[0] = static_cast<COLLISIONTYPE>(collidingBodies[0]->getUserIndex());
	collisionTypes[1] = static_cast<COLLISIONTYPE>(collidingBodies[1]->getUserIndex());

	// handle separation events object specific
	auto bikeIterator = std::find(collisionTypes.cbegin(), collisionTypes.cend(), BIKETYPE);
	if (bikeIterator != collisionTypes.cend())
	{
		int bikeIndex = bikeIterator - collisionTypes.cbegin();
		int otherIndex = bikeIndex == 0 ? 1 : 0;
		switch (collisionTypes[otherIndex])
		{
		case FENCETYPE:
		case LEVELTYPE:
		case LEVELGROUNDTYPE:
		case LEVELWALLTYPE:
		case BIKETYPE:
			break;
		default:
			break;
		}
	}
}