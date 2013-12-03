#pragma once
// bullet
#include <btBulletDynamicsCommon.h>
//troen
#include "../forwarddeclarations.h"
#include "abstractcontroller.h"

namespace troen
{
	class FenceController : public AbstractController
	{
	public:
		FenceController();
		void update(btVector3 position);
		void attachWorld(std::weak_ptr<PhysicsWorld>& world);
		void removeAllFences();

		virtual const COLLISIONTYPE getCollisionType() { return FENCETYPE; };

	private:
		btVector3 m_lastPosition;
		std::weak_ptr<PhysicsWorld> m_world;
	};
}