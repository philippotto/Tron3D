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
		GameLogic(TroenGame* game, std::shared_ptr<sound::AudioManager>& audioManager);

		// collision event functions
		virtual void collisionEvent(btRigidBody* pBody0, btRigidBody * pBody1, btPersistentManifold* contactManifold);
		virtual void separationEvent(btRigidBody * pBody0, btRigidBody * pBody1);

		virtual void handleCollisionOfBikeAndNonmovingObject(
			BikeController* bike,
			AbstractController* object,
			btPersistentManifold* contactManifold);
		virtual void handleCollisionOfTwoBikes(
			BikeController* bike1,
			BikeController* bike2,
			btPersistentManifold* contactManifold);

	private:
		std::shared_ptr<sound::AudioManager>m_audioManager;
		TroenGame*							m_troenGame;
	};
}