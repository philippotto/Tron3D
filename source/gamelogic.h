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
		GameLogic(
			TroenGame* game,
			std::shared_ptr<sound::AudioManager>& audioManager,
			std::shared_ptr<LevelController> levelController,
			std::vector<std::shared_ptr<Player>> players,
			const int timeLimit = 5);

		void attachPhysicsWorld(std::shared_ptr<PhysicsWorld>& physicsWorld);
		void step(const long double gameloopTime, const long double gameTime);

		typedef enum enum_GAMESTATE {
			GAME_START,
			GAME_RUNNING,
			GAME_OVER
		} GAMESTATE;

		inline GAMESTATE getGameState() { return m_gameState; };

		// collision event functions
		virtual void collisionEvent(btRigidBody* pBody0, btRigidBody * pBody1, btPersistentManifold* contactManifold);
		virtual void separationEvent(btRigidBody * pBody0, btRigidBody * pBody1);

		void removeAllFences();
		void toggleFencePartsLimit();
		void resetBike(BikeController *bikeController);
		void resetBikePositions();
		void restartLevel();

	private:
		virtual void handleCollisionOfBikeAndNonmovingObject(
			BikeController* bike,
			AbstractController* object,
			btPersistentManifold* contactManifold);
		virtual void handleCollisionOfTwoBikes(
			BikeController* bike1,
			BikeController* bike2,
			btPersistentManifold* contactManifold);

		std::shared_ptr<LevelController>	 m_levelController;
		std::vector<std::shared_ptr<Player>> m_players;

        TroenGame*							m_troenGame;
		std::shared_ptr<sound::AudioManager>m_audioManager;
		std::shared_ptr<PhysicsWorld>		m_physicsWorld;
        bool m_limitedFenceMode;
		
		GAMESTATE m_gameState;
		long double m_timeLimit;
		long double m_gameStartTime;
		void stepGameStart(const long double gameloopTime, const long double gameTime);
		void stepGameRunning(const long double gameloopTime, const long double gameTime);
		void stepGameOver(const long double gameloopTime, const long double gameTime);
	};
}