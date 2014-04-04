#pragma once
// OSG
#include <osg/ref_ptr>
// troen
#include "abstractcontroller.h"
#include "../forwarddeclarations.h"
#include "itemcontroller.h"
#include "../player.h"

namespace troen
{
	class RagdollController : public AbstractController
	{
	public:
		RagdollController(Player *player, btTransform startTransform);
		void attachWorld(std::shared_ptr<PhysicsWorld> &world);


	private:
		std::shared_ptr<RagdollView> m_ragdollView;
		std::shared_ptr<RagdollModel> m_ragdollModel;
		btVector3 m_startPosition;

		std::weak_ptr<PhysicsWorld> m_world;

		Player* m_player;
	};
}