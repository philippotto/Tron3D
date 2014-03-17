#include "GameLogic.h"
// STD
#include <array>
#include <random>
#include <chrono>
// bullet
#include <btBulletDynamicsCommon.h>
#include "LinearMath/btHashMap.h"
//troen
#include "troengame.h"
#include "constants.h"
#include "globals.h"
#include "player.h"
#include "model/abstractmodel.h"
#include "controller/abstractcontroller.h"
#include "controller/bikecontroller.h"
#include "controller/levelcontroller.h"
#include "controller/bikecontroller.h"
#include "controller/fencecontroller.h"
#include "controller/itemcontroller.h"
#include "sound/audiomanager.h"
#include "model/objectinfo.h"


using namespace troen;

GameLogic::GameLogic(TroenGame* game,const int timeLimit) :
t(game),
m_gameState(GAMESTATE::GAME_START),
m_timeLimit(timeLimit*1000*60),
m_gameStartTime(-1),
m_limitedFenceMode(true)
{}

////////////////////////////////////////////////////////////////////////////////
//
// Stepping
//
////////////////////////////////////////////////////////////////////////////////

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
		for (auto player : t->m_players)
			player->bikeController()->setState(BikeController::BIKESTATE::WAITING_FOR_GAMESTART,gameloopTime);
	}

	if (gameloopTime > m_gameStartTime)
	{
		for (auto player : t->m_players)
			player->bikeController()->setState(BikeController::BIKESTATE::DRIVING);
		m_gameState = GAMESTATE::GAME_RUNNING;
		m_gameStartTime = -1;
		t->unpauseSimulation();
	}
}

void GameLogic::stepGameRunning(const long double gameloopTime, const long double gameTime)
{
	if (gameTime >= m_timeLimit && m_timeLimit != 0)
	{
		m_gameState = GAMESTATE::GAME_OVER;
		t->pauseSimulation();
	}
}

void GameLogic::stepGameOver(const long double gameloopTime, const long double gameTime)
{
	;
}

////////////////////////////////////////////////////////////////////////////////
//
// Collision Event Handling
//
////////////////////////////////////////////////////////////////////////////////

void GameLogic::collisionEvent(btRigidBody * pBody0, btRigidBody * pBody1, btPersistentManifold* contactManifold)
{
	if (!pBody0->isInWorld() || !pBody1->isInWorld() || pBody0->getUserPointer() == nullptr || pBody1->getUserPointer() == nullptr) {
		return;
	}

	btRigidBody * collidingBodies[2];
	collidingBodies[0] = pBody0;
	collidingBodies[1] = pBody1;

	// get the controllers of the colliding objects
	ObjectInfo* objectInfos[2];
	objectInfos[0] = static_cast<ObjectInfo *>(pBody0->getUserPointer());
	objectInfos[1] = static_cast<ObjectInfo *>(pBody1->getUserPointer());

	// try to recognize invalid pointers - workaround for debugmode
	if (objectInfos[0] == (void*)0xfeeefeeefeeefeee || objectInfos[1] == (void*)0xfeeefeeefeeefeee) return;

	AbstractController* collisionBodyControllers[2];
	try {
		collisionBodyControllers[0] = objectInfos[0]->getUserPointer();
		collisionBodyControllers[1] = objectInfos[1]->getUserPointer();
	}
	catch (int e) {
		std::cout << "RigidBody invalid, but pointer was not 0xfeeefeeefeeefeee: " << e << std::endl;
		return;
	}

	// exit if either controller was not found
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
			handleCollisionOfBikeAndNonmovingObject(
				dynamic_cast<BikeController*>(collisionBodyControllers[bikeIndex]),
				collisionBodyControllers[otherIndex],
				collisionTypes[otherIndex],
				contactManifold);
			break;

		case FENCETYPE:
			handleCollisionOfBikeAndFence(
				dynamic_cast<BikeController*>(collisionBodyControllers[bikeIndex]),
				dynamic_cast<FenceController*>(collisionBodyControllers[otherIndex]),
				contactManifold);
			break;

		case BIKETYPE:
			handleCollisionOfTwoBikes(
				dynamic_cast<BikeController*>(collisionBodyControllers[bikeIndex]),
				dynamic_cast<BikeController*>(collisionBodyControllers[otherIndex]),
				contactManifold);
			break;

		case ITEMTYPE:
			handleCollisionOfBikeAndItem(
				dynamic_cast<BikeController*>(collisionBodyControllers[bikeIndex]),
				dynamic_cast<ItemController *>(collisionBodyControllers[otherIndex]));
			break;

		case LEVELGROUNDTYPE:
			break;
		default:
			break;
		}
	}
}

void GameLogic::separationEvent(btRigidBody * pBody0, btRigidBody * pBody1)
{
	if (!pBody0->isInWorld() || !pBody1->isInWorld() || pBody0->getUserPointer() == nullptr || pBody1->getUserPointer() == nullptr) {
		return;
	}

	btRigidBody * collidingBodies[2];
	collidingBodies[0] = pBody0;
	collidingBodies[1] = pBody1;

	ObjectInfo* objectInfos[2];
	objectInfos[0] = static_cast<ObjectInfo *>(pBody0->getUserPointer());
	objectInfos[1] = static_cast<ObjectInfo *>(pBody1->getUserPointer());

	// try to recognize invalid pointers - workaround for debugmode
	if (objectInfos[0] == (void*)0xfeeefeeefeeefeee || objectInfos[1] == (void*)0xfeeefeeefeeefeee) return;

	// get the controllers of the separating objects
	AbstractController* collisionBodyControllers[2];
	collisionBodyControllers[0] = objectInfos[0]->getUserPointer();
	collisionBodyControllers[1] = objectInfos[1]->getUserPointer();

	// exit if either controller was not found
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
		{
						  // workaround to deal with bike bouncing between own and other fence
						  FenceController * fence = dynamic_cast<FenceController*>(collisionBodyControllers[otherIndex]);
						  BikeController * bike = dynamic_cast<BikeController*>(collisionBodyControllers[bikeIndex]);
						  bike->rememberFenceCollision(fence);
						  // end workaround
		}
			break;
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

void GameLogic::handleCollisionOfTwoBikes(
	BikeController* bike1,
	BikeController* bike2,
	btPersistentManifold* contactManifold)
{
	std::cout << "[GameLogic::handleCollisionOfTwoBikes]" << std::endl;
	//TODO
	// set different thredsholds of collisions between bikes
	// they dont have as much impact ?
	handleCollisionOfBikeAndNonmovingObject(bike1, bike2, BIKETYPE, contactManifold);
}

void GameLogic::handleCollisionOfBikeAndItem(
	BikeController* bike,
	ItemController* item)
{
	if (item)
	{
		item->triggerOn(bike,this);
	}
}

void GameLogic::handleCollisionOfBikeAndNonmovingObject(
	BikeController* bike,
	AbstractController* object,
	const int objectType,
	btPersistentManifold* contactManifold)
{
	btScalar impulse = impulseFromContactManifold(contactManifold);

	playCollisionSound(impulse);
	float newHealth = bike->registerCollision(impulse);

	//
	// player death
	//
	if (newHealth <= 0 && bike->state() == BikeController::BIKESTATE::DRIVING)
	{
		handlePlayerDeath(bike);
		handlePlayerDeathNonFence(bike->player());
	}
}

void GameLogic::handleCollisionOfBikeAndFence(
	BikeController* bike,
	FenceController* fence,
	btPersistentManifold* contactManifold)
{

	if (bike->player()->isRemote())
		return; //let the remote player handle the collsions himself

	btScalar impulse = impulseFromContactManifold(contactManifold);

	playCollisionSound(impulse);
	float newHealth = bike->registerCollision(impulse);

	// workaround to deal with bike bouncing between own and other fence
	if (bike->player() != fence->player())
	{
		bike->rememberFenceCollision(fence);
	}
	// end workaround

	//
	// player death
	//
	if (newHealth <= 0 && bike->state() == BikeController::BIKESTATE::DRIVING)
	{
		handlePlayerDeath(bike);
		handlePlayerDeathOnFence(fence->player(), bike->player());
	}
}

void GameLogic::handlePlayerDeath(
	BikeController* bike)
{
	bike->player()->increaseDeathCount();
	bike->setState(BikeController::BIKESTATE::RESPAWN, g_gameTime);
}

void GameLogic::handlePlayerDeathOnFence(
	Player* fencePlayer,
	Player* bikePlayer)
{

	if (fencePlayer == bikePlayer) // hit own fence
	{
		// workaround to deal with bike bouncing between own and other fence
		std::pair<float, FenceController*> lastFenceCollision =	bikePlayer->bikeController()->lastFenceCollision();
		if (false && lastFenceCollision.first > g_gameTime-400)
		{
			handlePlayerDeathOnFence(lastFenceCollision.second->player(), bikePlayer);
			return;
		}
		// end workaround

		bikePlayer->decreaseKillCount();
		if (bikePlayer->hasGameView())
		{
			bikePlayer->hudController()->addSelfKillMessage();
		}
	}
	else //hit someone elses fence
	{

		fencePlayer->increaseKillCount();
		for (auto player : t->m_playersWithView)
		{
			if (&(*player) == fencePlayer)
			{
				fencePlayer->hudController()->addKillMessage(bikePlayer);
			}
			else if (&(*player) != bikePlayer)
			{
				player->hudController()->addDiedOnFenceMessage(bikePlayer, fencePlayer);
			}
		}
	}
}

void GameLogic::handlePlayerDeathNonFence(Player* deadPlayer)
{

	for (auto player : t->m_players)
	{
		if (&(*player) != deadPlayer)
		{
			player->increaseKillCount();
			if (player->hasGameView())
			{
				player->hudController()->addDiedMessage(deadPlayer);
			}
		}
	}
}

btScalar GameLogic::impulseFromContactManifold(btPersistentManifold* contactManifold)
{
	btScalar impulse = 0;
	int numContacts = contactManifold->getNumContacts();
	//std::cout << numContacts << " - ";
	for (int i = 0; i < numContacts; i++)
	{
		btManifoldPoint& pt = contactManifold->getContactPoint(i);
		if (abs(pt.m_normalWorldOnB.z()) < 0.5)
		{
			// ignore collisions with xy-plane (so the player doesn't lose health if he drives on a surface)
			impulse = impulse + pt.getAppliedImpulse();
		}
	}
	
	return impulse;
}

////////////////////////////////////////////////////////////////////////////////
//
// logic methods
//
////////////////////////////////////////////////////////////////////////////////

void GameLogic::removeAllFences()
{
	for (auto player : t->m_players)
	{
		player->fenceController()->removeAllFences();
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

	for (auto player : t->m_players)
	{
		player->fenceController()->setLimitFence(m_limitedFenceMode);
	}
}

void GameLogic::resetBike(BikeController *bikeController)
{
	bikeController->reset();

	// TODO should happen in reset()
	btTransform position = t->m_levelController->getRandomSpawnPoint();
	bikeController->moveBikeToPosition(position);
}

void GameLogic::resetBikePositions()
{
	for (auto player : t->m_players)
	{
		btTransform position = t->m_levelController->getSpawnPointForBikeWithIndex(player->id());
		player->bikeController()->moveBikeToPosition(position);
	}
}

void GameLogic::restartLevel()
{
	removeAllFences();
	resetBikePositions();
}

void GameLogic::playCollisionSound(float impulse)
{
	if (impulse > BIKE_FENCE_IMPACT_THRESHOLD_LOW)
	{
		t->m_audioManager->PlaySFX("data/sound/explosion.wav",
			impulse / BIKE_FENCE_IMPACT_THRESHOLD_HIGH,
			impulse / (BIKE_FENCE_IMPACT_THRESHOLD_HIGH - BIKE_FENCE_IMPACT_THRESHOLD_LOW),
			1, 1);
	}
}

void GameLogic::hideFencesInRadarForPlayer(int id)
{
	for (auto player : t->m_players)
	{
		player->fenceController()->hideFencesInRadarForPlayer(id);
	}
}

void GameLogic::showFencesInRadarForPlayer(int id)
{
	for (auto player : t->m_players)
	{
		player->fenceController()->showFencesInRadarForPlayer(id);
	}
}
