#pragma once
// STD
#include <vector>
#include <set>
#include <iterator>
#include <algorithm>
// troen
#include "forwarddeclarations.h"
#include "network/networkmanager.h"
#include "qqueue.h"

// typedefs for collision events

namespace troen
{
	class GameLogic
	{

	public:
		typedef enum enum_GAMESTATE {
			GAME_START,
			GAME_RUNNING,
			GAME_OVER
		} GAMESTATE;

		GameLogic(TroenGame* game, const int timeLimit = 5);
		void attachPhysicsWorld(std::shared_ptr<PhysicsWorld>& physicsWorld);

		//
		// stepping
		//
		void step(const long double gameloopTime, const long double gameTime);
		GAMESTATE getGameState() { return m_gameState; };

		//
		// collision event handling
		//
		virtual void collisionEvent(btRigidBody* pBody0, btRigidBody * pBody1, btPersistentManifold* contactManifold);
		virtual void separationEvent(btRigidBody * pBody0, btRigidBody * pBody1);

		//
		// logic methods
		//
		void removeAllFences();
		void toggleFencePartsLimit();
		void hideFencesInRadarForPlayer(int id);
		void showFencesInRadarForPlayer(int id);

		void resetBike(BikeController *bikeController);
		void resetBikePositions();

		void restartLevel();
		
		//networking
		void handleNetworkMessage(troen::networking::gameStatus status, Player *bikePlayer, Player *fencePlayer);
		void sendStatusMessage(troen::networking::gameStatus status, Player *bikePlayer, Player *fencePlayer);
		void processNetworkMessages();

	
		//
		// collision event handling
		//
		void handleCollisionOfBikeAndNonmovingObject(
			BikeController* bike,
			AbstractController* object,
			const int objectType,
			btPersistentManifold* contactManifold);
		void handleCollisionOfBikeAndFence(
			BikeController* bike,
			FenceController* fence,
			btPersistentManifold* contactManifold);
		void handleCollisionOfTwoBikes(
			BikeController* bike1,
			BikeController* bike2,
			btPersistentManifold* contactManifold);
		void handleCollisionOfBikeAndItem(
			BikeController* bike,
			ItemController* item);
		// death handling
		void handlePlayerDeath(BikeController* bike);
		void handlePlayerDeathOnFence(BikeController* fenceBike, BikeController* deadBike);
		void handlePlayerDeathNonFence(BikeController* deadBike);
		void handlePlayerFall(BikeController* deadBike);

		private:
		// helper
		float impulseFromContactManifold(btPersistentManifold* contactManifold);
		void playCollisionSound(float impulse);
		Player* getPlayerWithID(int bikeID);

		void checkForFallenPlayers();
		//
		// communication links
		//
		TroenGame *m_troenGame;
		QQueue<networking::gameStatusMessage> *m_receivedGameMessages;

		//
		// stepping variables & methods
		//
		GAMESTATE m_gameState;
		long double m_timeLimit;
		long double m_gameStartTime;
		void stepGameStart(
			const long double gameloopTime,
			const long double gameTime);
		void stepGameRunning(
			const long double gameloopTime,
			const long double gameTime);
		void stepGameOver(
			const long double gameloopTime,
			const long double gameTime);
		bool m_limitedFenceMode;
	};
}