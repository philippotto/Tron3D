#include "PhysicsWorld.h"
// STD
#include <array>
// bullet
#include <btBulletDynamicsCommon.h>
#include "LinearMath/btHashMap.h"
//troen
#include "../constants.h"
#include "../gamelogic.h"
#include "../util/gldebugdrawer.h"
#include "../model/abstractmodel.h"
#include "../controller/bikecontroller.h"
#include "../sound/audiomanager.h"

#include <ctime>

using namespace troen;


PhysicsWorld::PhysicsWorld(
	std::shared_ptr<GameLogic>& gameLogic, 
	bool useDebugView) :
m_lastSimulationTime(0),
m_useDebugView(useDebugView),
m_gameLogic(gameLogic)
{
	initializeWorld();

	if (m_useDebugView)
	{
		m_debug = new util::GLDebugDrawer();
		m_debug->setDebugMode(btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);
		m_world->setDebugDrawer(m_debug);
	}	
}

std::array<std::array<int, 100>, 100>* PhysicsWorld::discretizeWorld()
{
	const float halfedLevelSize = LEVEL_SIZE / 2;
	const float discreteLevelSize = 100;
	const float stepSize = LEVEL_SIZE / discreteLevelSize;
	return &m_discretizedWorld;
	// print && clear

	for (int x = 0; x < discreteLevelSize; x++) {
		for (int y = 0; y < discreteLevelSize; y++) {
			std::cout << " " << m_discretizedWorld[y][x];
			m_discretizedWorld[y][x] = 0;

		}
		std::cout << std::endl;
	}
	std::cout << std::endl;



	
	btCollisionObjectArray objects = m_world->getCollisionObjectArray();
	for (size_t i = 0; i < objects.size(); i++)
	{
		btCollisionObject* object = objects[i];
		btTransform pos = object->getWorldTransform();
		pos.getOrigin();
		btCollisionShape* shape = object->getCollisionShape();
		//shape->get

	}
	









	// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	btVector3 from, to, direction;
	// return &m_discretizedWorld;
	clock_t start, end;
	start = std::clock();

	// clear

	for (int x = 0; x < discreteLevelSize; x++) {
		for (int y = 0; y < discreteLevelSize; y++) {
			std::cout << " " << m_discretizedWorld[y][x];
			m_discretizedWorld[y][x] = 0;
			
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;


	// fill

	float y, x;

	from.setY(-halfedLevelSize);
	to.setY(+halfedLevelSize);
	from.setZ(1.1);
	to.setZ(1.1);

	int weirdAnswers = 0;

	for (x = -halfedLevelSize; x < halfedLevelSize; x += stepSize) {
		from.setX(x);
		to.setX(x);
		
		btCollisionWorld::AllHitsRayResultCallback RayCallback(from, to);
		m_world->rayTest(from, to, RayCallback);
		if (RayCallback.hasHit()) {
			btAlignedObjectArray<btVector3> collisionPoints = RayCallback.m_hitPointWorld;
			std::cout << "length of collisionpoints: " << collisionPoints.size() << std::endl;
			for (int index = 0; index < collisionPoints.size(); index++) {
				btVector3 position = collisionPoints.at(index);

				if (false && (abs(position.x()) > halfedLevelSize || abs(position.y()) != halfedLevelSize)) {
					std::cout << "weird answer of bullet" << position.x() << "  " << position.y() << "  " << position.z() << std::endl;
					weirdAnswers++;
				}
				else {

					int hitY = (position.y() + halfedLevelSize) / stepSize;
					int hitX = (x + halfedLevelSize) / stepSize;

					if (hitY < 0 || hitX < 0 || hitY >= discreteLevelSize || hitX >= discreteLevelSize) {
						std::cout << "hitY: " << hitY << " hitX: " << hitX << std::endl;
					}
					else
					{
						m_discretizedWorld[hitY][hitX] = 1;
					}
				}
			}
		}
	}



	from.setX(-halfedLevelSize);
	to.setX(+halfedLevelSize);
	from.setZ(1.1);
	to.setZ(1.1);


	for (y = -LEVEL_SIZE / 2; y < LEVEL_SIZE / 2; y += stepSize) {
		from.setY(y);
		to.setY(y);

		btVector3 from, to, direction;
		btCollisionWorld::AllHitsRayResultCallback RayCallback(from, to);
		m_world->rayTest(from, to, RayCallback);
		if (RayCallback.hasHit()) {
			btAlignedObjectArray<btVector3> collisionPoints = RayCallback.m_hitPointWorld;
			std::cout << "length of collisionpoints: " << collisionPoints.size() << std::endl;

			for (int index = 0; index < collisionPoints.size(); index++) {
				btVector3 position = collisionPoints.at(index);

				if (false && (abs(position.y()) > halfedLevelSize || abs(position.x()) != halfedLevelSize)) {
					std::cout << "weird answer of bullet" << position.x() << "  " << position.y() << "  " << position.z() << std::endl << std::endl;
					weirdAnswers++;
				} else {

					int hitX = (position.x() + halfedLevelSize) / stepSize;
					int hitY = (y + halfedLevelSize) / stepSize;
					if (hitY < 0 || hitX < 0 || hitY >= discreteLevelSize || hitX >= discreteLevelSize) {
						std::cout << "hitY: " << hitY << " hitX: " << position.x() << "  " << position.y() << "  " << position.z() << std::endl;
					}
					else {
						m_discretizedWorld[hitY][hitX] = 1;
					}
				}
			}
		}
	}
	//}
	end = std::clock();
	double dif = double(end - start) / CLOCKS_PER_SEC;
	//printf("Elasped time is %.2lf seconds.", dif);

	std::cout << "weird answers in total: " << weirdAnswers << std::endl;

	return &m_discretizedWorld;
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

	m_world->setGravity(DEFAULT_GRAVITY);
}

void PhysicsWorld::addRigidBodies(const std::vector<std::shared_ptr<btRigidBody>>& bodies, const short group/*=0*/, const short mask/*=0*/)
{
	for (auto body : bodies)
	{
		m_world->addRigidBody(body.get(), group, mask);
	}
}

void PhysicsWorld::addRigidBody(btRigidBody *body, const short group /*=0*/, const short mask /*=0*/)
{
	m_world->addRigidBody(body, group, mask);
}

void PhysicsWorld::removeRigidBodies(const std::vector<std::shared_ptr<btRigidBody>>& bodies)
{
	for (auto body : bodies) {
		removeRigidBodyFromCollisionPairs(body.get());
		m_world->removeRigidBody(body.get());
	}
}

void PhysicsWorld::removeRigidBodyFromCollisionPairs(btRigidBody* body)
{
	for (CollisionPairSet::iterator iter = m_pairsLastUpdate.begin(); iter != m_pairsLastUpdate.end(); /* no increment */) {
		if ((btRigidBody*)iter->first == body || (btRigidBody*)iter->second == body) {
			m_pairsLastUpdate.erase(iter++);
		}
		else {
			++iter;
		}
	}
	m_removedRigidBodies.push_back(body);
}


void PhysicsWorld::removeRigidBody(btRigidBody* body)
{
	removeRigidBodyFromCollisionPairs(body); 
	m_world->removeRigidBody(body);
}

void PhysicsWorld::removeCollisionObject(btCollisionObject* obj)
{
	m_world->removeCollisionObject(obj);
}

void PhysicsWorld::addCollisionObject(btCollisionObject* obj)
{
	m_world->addCollisionObject(obj);
}

void PhysicsWorld::stepSimulation(long double currentTime)
{	
	float timeSinceLastSimulation = currentTime - m_lastSimulationTime;
	m_lastSimulationTime = currentTime;

	// careful:
	// including this debug-printout will significantly reduce
	// framerate & flow of the game
	//std::cout << "[PhysicsWorld::stepSimulation] timeSinceLastSimulation = " << timeSinceLastSimulation/1000 << std::endl;
	
	if (m_useDebugView)
	{
		m_debug->BeginDraw();
	}
	
	// mind the following constraint:
	// timeStep < maxSubSteps * fixedTimeStep
	// where the parameters are given as follows:
	// stepSimulation(timeStep, maxSubSteps, fixedTimeStep)
	m_world->stepSimulation(timeSinceLastSimulation/1000.f, 8);
	
	if (m_useDebugView)
	{
		m_world->debugDrawWorld();
		m_debug->EndDraw();
	}	

	checkForCollisionEvents();

	// discretizeWorld();

}

void PhysicsWorld::checkForCollisionEvents()
{
	// keep a list of the collision pairs we
	// found during the current update
	CollisionPairSet pairsThisUpdate;
	
	// iterate through all of the manifolds in the dispatcher
	for (int i = 0; i < m_dispatcher->getNumManifolds(); ++i) {
		
		// get the manifold
		btPersistentManifold* contactManifold = m_dispatcher->getManifoldByIndexInternal(i);

		// ignore manifolds that have no contact points.
		if (contactManifold->getNumContacts() > 0)
		{
			// get the two rigid bodies involved in the collision
			const btRigidBody* pBody0 = static_cast<const btRigidBody*>(contactManifold->getBody0());
			const btRigidBody* pBody1 = static_cast<const btRigidBody*>(contactManifold->getBody1());
			
			// create the pair in a predictable order (using the pointer value)
			bool const swapped = pBody0 > pBody1;
			const btRigidBody* pSortedBodyA = swapped ? pBody1 : pBody0;
			const btRigidBody* pSortedBodyB = swapped ? pBody0 : pBody1;
			
			bool notJustRemoved = true;
			for (auto removedRigidBody : m_removedRigidBodies) {
				if (pSortedBodyA == removedRigidBody || pSortedBodyB == removedRigidBody) {
					notJustRemoved = false;
					//std::cout << "A rigidBody was just removed and prevented from doing bad things." << std::endl;
					break;
				}
			}

			if (notJustRemoved) {
				// create the pair
				CollisionPair thisPair = std::make_pair(pSortedBodyA, pSortedBodyB);

				// insert the pair into the current list
				pairsThisUpdate.insert(thisPair);

				// if this pair doesn't exist in the list
				// from the previous update, it is a new
				// pair and we must send a collision event
				if (m_pairsLastUpdate.find(thisPair) == m_pairsLastUpdate.end())
					m_gameLogic.lock()->collisionEvent((btRigidBody*)pBody0, (btRigidBody*)pBody1, contactManifold);
			}
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
	for (CollisionPairSet::const_iterator iter = removedPairs.begin(); iter != removedPairs.end(); ++iter) {
		m_gameLogic.lock()->separationEvent((btRigidBody*)iter->first, (btRigidBody*)iter->second);
	}
	
	// in the next iteration we'll want to compare against
	// the pairs we found in this iteration
	m_pairsLastUpdate = pairsThisUpdate;
	m_removedRigidBodies.clear();
}


