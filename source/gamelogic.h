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
			std::vector<std::shared_ptr<BikeController>> bikeControllers);

		void attachPhysicsWorld(std::shared_ptr<PhysicsWorld>& physicsWorld);

		// collision event functions
		virtual void collisionEvent(btRigidBody* pBody0, btRigidBody * pBody1, btPersistentManifold* contactManifold);
		virtual void separationEvent(btRigidBody * pBody0, btRigidBody * pBody1);

		void removeAllFences();
		void toggleFencePartsLimit();

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

		int m_maxFenceParts;

		std::shared_ptr<LevelController>			 m_levelController;
		std::vector<std::shared_ptr<BikeController>> m_bikeControllers;

		std::shared_ptr<sound::AudioManager>m_audioManager;
		std::shared_ptr<PhysicsWorld>		m_physicsWorld;
		TroenGame*							m_troenGame;
	};
}