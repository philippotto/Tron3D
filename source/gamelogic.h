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
		GameLogic(std::shared_ptr<sound::AudioManager>& audioManager);

		// collision event functions
		virtual void collisionEvent(btRigidBody* pBody0, btRigidBody * pBody1, btPersistentManifold* contactManifold);
		virtual void separationEvent(btRigidBody * pBody0, btRigidBody * pBody1);

	private:
		std::shared_ptr<sound::AudioManager>m_audioManager;
		std::weak_ptr<TroenGame>			m_troenGame;
		std::weak_ptr<PhysicsWorld>			m_physicsWorld;

	};
}