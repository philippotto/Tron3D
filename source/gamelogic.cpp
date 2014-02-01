#include "GameLogic.h"
// STD
#include <array>
// bullet
#include <btBulletDynamicsCommon.h>
#include "LinearMath/btHashMap.h"
//troen
#include "troengame.h"
#include "constants.h"
#include "globals.h"
#include "model/abstractmodel.h"
#include "controller/bikecontroller.h"
#include "controller/levelcontroller.h"
#include "controller/bikecontroller.h"
#include "controller/itemcontroller.h"
#include "sound/audiomanager.h"
#include "model/objectinfo.h"


using namespace troen;

GameLogic::GameLogic(
	TroenGame* game,
	std::shared_ptr<sound::AudioManager>& audioManager,
	std::shared_ptr<LevelController> levelController,
	std::vector<std::shared_ptr<BikeController>> bikeControllers) :
m_levelController(levelController),
m_bikeControllers(bikeControllers),
m_troenGame(game),
m_limitedFenceMode(true),
m_audioManager(audioManager),
m_gameState(GAMESTATE::GAME_START),
m_gameStartTime(-1)
{}

void GameLogic::attachPhysicsWorld(std::shared_ptr<PhysicsWorld>& physicsWorld)
{
	m_physicsWorld = physicsWorld;
}

void GameLogic::step(const long double gameloopTime, const long double gameTime)
{
	switch (m_gameState)
	{
	case GAME_START:
		stepGameStart(gameloopTime, gameTime);
		break;
	case GAME_RUNNING:
		stepGameRunning(gameloopTime, gameTime);
		break;
	case GAME_OVER:
		stepGameOver(gameloopTime, gameTime);
		break;
	default:
		break;
	}
}

void GameLogic::stepGameStart(const long double gameloopTime, const long double gameTime)
{
	if (m_gameStartTime == -1)
	{
		m_gameStartTime = gameloopTime + GAME_START_COUNTDOWN_DURATION;
		for (auto bikeController : m_bikeControllers)
			bikeController->setState(BikeController::BIKESTATE::WAITING_FOR_GAMESTART,gameloopTime);
	}

	if (gameloopTime > m_gameStartTime)
	{
		for (auto bikeController : m_bikeControllers)
			bikeController->setState(BikeController::BIKESTATE::DRIVING);
		m_gameState = GAMESTATE::GAME_RUNNING;
		m_gameStartTime = -1;
		m_troenGame->unpauseSimulation();
	}
}

void GameLogic::stepGameRunning(const long double gameloopTime, const long double gameTime)
{
	;
}

void GameLogic::stepGameOver(const long double gameloopTime, const long double gameTime)
{
	;
}


void GameLogic::collisionEvent(btRigidBody * pBody0, btRigidBody * pBody1, btPersistentManifold* contactManifold)
{
	//std::cout << "[PhysicsWorld::collisionEvent] collision detected" << std::endl;
	btRigidBody * collidingBodies[2];
	collidingBodies[0] = pBody0;
	collidingBodies[1] = pBody1;

	// get the controllers of the colliding objects
	ObjectInfo* objectInfos[2];
	objectInfos[0] = static_cast<ObjectInfo *>(pBody0->getUserPointer());
	objectInfos[1] = static_cast<ObjectInfo *>(pBody1->getUserPointer());

	// try to recognize invalid pointers
	if (objectInfos[0] == (void*)0xfeeefeeefeeefeee || objectInfos[1] == (void*)0xfeeefeeefeeefeee) return;

	AbstractController* collisionBodyControllers[2];
	try {
		collisionBodyControllers[0] = static_cast<AbstractController *>(objectInfos[0]->getUserPointer());
		collisionBodyControllers[1] = static_cast<AbstractController *>(objectInfos[1]->getUserPointer());
	}
	catch (int e) {
		std::cout << "RigidBody invalid, but pointer was not 0xfeeefeeefeeefeee: " << e << std::endl;
		return;
	}

	// exit either controllers was not found
	if (!collisionBodyControllers[0] || !collisionBodyControllers[1]) return;

	std::array<COLLISIONTYPE,2> collisionTypes;
	collisionTypes[0] = static_cast<COLLISIONTYPE>(objectInfos[0]->getUserIndex());
	collisionTypes[1] = static_cast<COLLISIONTYPE>(objectInfos[1]->getUserIndex());


	// handle colision events object specific
	auto bikeIterator = std::find(collisionTypes.cbegin(), collisionTypes.cend(), BIKETYPE);
	if (bikeIterator != collisionTypes.cend())
	{
		int bikeIndex = bikeIterator - collisionTypes.cbegin();
		int otherIndex = bikeIndex == 0 ? 1 : 0;
		switch (collisionTypes[otherIndex])
		{
		case LEVELWALLTYPE:
		case LEVELOBSTACLETYPE:
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
			break;
		case ITEMTYPE:
		{
			ItemController* itemController = static_cast<ItemController *>(collisionBodyControllers[otherIndex]);
			if (itemController) {
				itemController->triggerOn(static_cast<BikeController*>(collisionBodyControllers[bikeIndex]));
			}
			break;
		}
		default:
			break;
		}
	}
}

void GameLogic::separationEvent(btRigidBody * pBody0, btRigidBody * pBody1)
{
	return;
	//std::cout << "[PhysicsWorld::seperationEvent] seperation detected" << std::endl;
	btRigidBody * collidingBodies[2];
	collidingBodies[0] = pBody0;
	collidingBodies[1] = pBody1;

	ObjectInfo* objectInfos[2];
	objectInfos[0] = static_cast<ObjectInfo *>(pBody0->getUserPointer());
	objectInfos[1] = static_cast<ObjectInfo *>(pBody1->getUserPointer());

	// get the controllers of the separating objects
	AbstractController* collisionBodyControllers[2];
	collisionBodyControllers[0] = static_cast<AbstractController *>(objectInfos[0]->getUserPointer());
	collisionBodyControllers[1] = static_cast<AbstractController *>(objectInfos[1]->getUserPointer());

	// exit either controlles was not found
	if (!collisionBodyControllers[0] || !collisionBodyControllers[1]) return;

	std::array<COLLISIONTYPE, 2> collisionTypes;
	collisionTypes[0] = static_cast<COLLISIONTYPE>(objectInfos[0]->getUserIndex());
	collisionTypes[1] = static_cast<COLLISIONTYPE>(objectInfos[1]->getUserIndex());

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
		case LEVELOBSTACLETYPE:
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
	//std::cout << "total impulse: " << impulse << std::endl;
	if (impulse > BIKE_FENCE_IMPACT_THRESHOLD_LOW)
		m_audioManager->PlaySFX("data/sound/explosion.wav",
			impulse / BIKE_FENCE_IMPACT_THRESHOLD_HIGH,
			impulse / (BIKE_FENCE_IMPACT_THRESHOLD_HIGH - BIKE_FENCE_IMPACT_THRESHOLD_LOW),
			1, 1);


	bike->registerCollision(impulse);
	// TODO (Philipp): move increaseHealth and resetBike into registerCollision and trigger a respawn instead of pausing simulation (needs statistics about lifes etc.)
	float newHealth = bike->increaseHealth(-1 * impulse);

	if (newHealth <= 0)
	{
		//resetBike(bike);
		//m_troenGame->pauseSimulation();
		//restartLevel();
		bike->setState(BikeController::BIKESTATE::RESPAWN, g_currentTime);
	}
}

void GameLogic::handleCollisionOfTwoBikes(
	BikeController* bike1,
	BikeController* bike2,
	btPersistentManifold* contactManifold)
{
	std::cout << "[GameLogic::handleCollisionOfTwoBikes]" << std::endl;
	//TODO
	// set different thredsholds of collisions between bikes
	// they dont have as much impact ?
	handleCollisionOfBikeAndNonmovingObject(bike1, bike2, contactManifold);
}

void GameLogic::removeAllFences()
{
	for (auto bikeController : m_bikeControllers)
	{
		bikeController->removeAllFences();
	}
}

void GameLogic::toggleFencePartsLimit()
{
	m_limitedFenceMode = !m_limitedFenceMode;
	if (m_limitedFenceMode){
		std::cout << "[GameLogic::toggleFencePartsLimitEvent] turning fenceParsLimit ON ..." << std::endl;
	}
	else
	{
		std::cout << "[GameLogic::toggleFencePartsLimitEvent] turning fenceParsLimit OFF ..." << std::endl;
	}

	for (auto bikeController : m_bikeControllers)
	{
		bikeController->setLimitFence(m_limitedFenceMode);
	}
}

void GameLogic::resetBike(BikeController *bikeController)
{
	bikeController->reset();

	// TODO should happen in reset()
	btTransform position = m_levelController->getRandomSpawnPoint();
	bikeController->moveBikeToPosition(position);
}

void GameLogic::resetBikePositions()
{
	for (int i = 0; i < m_bikeControllers.size(); i++)
	{
		btTransform position = m_levelController->getSpawnPointForBikeWithIndex(i);
		m_bikeControllers[i]->moveBikeToPosition(position);
	}
}

void GameLogic::restartLevel()
{
	removeAllFences();
	resetBikePositions();
}
