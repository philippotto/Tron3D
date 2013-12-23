#include "GameLogic.h"
// STD
#include <array>
// bullet
#include <btBulletDynamicsCommon.h>
#include "LinearMath/btHashMap.h"
//troen
#include "troengame.h"
#include "constants.h"
#include "model/abstractmodel.h"
#include "controller/bikecontroller.h"
#include "sound/audiomanager.h"

using namespace troen;

GameLogic::GameLogic(TroenGame* game, std::shared_ptr<sound::AudioManager>& audioManager) :
m_troenGame(game),
m_audioManager(audioManager)
{}

void GameLogic::collisionEvent(btRigidBody * pBody0, btRigidBody * pBody1, btPersistentManifold* contactManifold)
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
		case LEVELWALLTYPE:
		case FENCETYPE:
			handleCollisionOfBikeAndNonmovingObject(
				static_cast<BikeController*>(collisionBodyControllers[bikeIndex]),
				collisionBodyControllers[otherIndex],
				contactManifold);
			break;
		case BIKETYPE:
			handleCollisionOfTwoBikes(
				static_cast<BikeController*>(collisionBodyControllers[bikeIndex]),
				static_cast<BikeController*>(collisionBodyControllers[otherIndex]),
				contactManifold);
			break;
		case LEVELGROUNDTYPE:
			//std::cout << "collision with ground" << std::endl;
		default:
			break;
		}
	}
}

void GameLogic::separationEvent(btRigidBody * pBody0, btRigidBody * pBody1)
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

void GameLogic::handleCollisionOfBikeAndNonmovingObject(
	BikeController* bike,
	AbstractController* object,
	btPersistentManifold* contactManifold)
{
	btScalar impulse = 0;
	int numContacts = contactManifold->getNumContacts();
	//std::cout << numContacts << " - ";
	for (int i = 0; i < numContacts; i++)
	{
		btManifoldPoint& pt = contactManifold->getContactPoint(i);
		impulse = impulse + pt.getAppliedImpulse();
	}
	std::cout << "total impulse: " << impulse << std::endl;
	if (impulse > BIKE_IMPACT_THRESHOLD_LOW)
		m_audioManager->PlaySFX("data/sound/explosion.wav",
		impulse / BIKE_IMPACT_THRESHOLD_HIGH,
		impulse / (BIKE_IMPACT_THRESHOLD_HIGH - BIKE_IMPACT_THRESHOLD_LOW),
		1, 1);

	if (impulse > BIKE_IMPACT_THRESHOLD_HIGH)
	{
		m_troenGame->pauseSimulation();
	}
}

void GameLogic::handleCollisionOfTwoBikes(
	BikeController* bike1,
	BikeController* bike2,
	btPersistentManifold* contactManifold)
{
	std::cout << "[GameLogic::handleCollisionOfTwoBikes]" << std::endl;
}