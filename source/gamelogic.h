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

		GameLogic(
			TroenGame* game,
			std::shared_ptr<sound::AudioManager>& audioManager,
			std::shared_ptr<LevelController> levelController,
			std::vector<std::shared_ptr<Player>> players,
			const int timeLimit = 5);
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
		virtual void handleCollisionOfBikeAndNonmovingObject(
			BikeController* bike,
			AbstractController* object,
			btPersistentManifold* contactManifold);
		virtual void handleCollisionOfTwoBikes(
			BikeController* bike1,
			BikeController* bike2,
			btPersistentManifold* contactManifold);

		//
		// communication links
		//
		TroenGame*							m_troenGame;
		std::shared_ptr<PhysicsWorld>		m_physicsWorld;
		std::vector<std::shared_ptr<Player>> m_players;
		std::shared_ptr<LevelController>	 m_levelController;
		std::shared_ptr<sound::AudioManager>m_audioManager;
		
		//
		// Stepping variables & helper methods
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