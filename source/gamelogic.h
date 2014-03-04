#pragma once
// STD
#include <vector>
#include <set>
#include <iterator>
#include <algorithm>
// troen
#include "forwarddeclarations.h"

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
		void resetBike(BikeController *bikeController);
		void resetBikePositions();
		void restartLevel();

	private:
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
		void handlePlayerDeathOnFence(Player* fencePlayer, Player* bikePlayer);
		void handlePlayerDeathNonFence(	Player* deadPlayer);
		// helper
		float impulseFromContactManifold(btPersistentManifold* contactManifold);
		void playCollisionSound(float impulse);

		//
		// communication links
		//
		TroenGame*t;
		
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