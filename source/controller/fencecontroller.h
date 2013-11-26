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
		void attachWorld(std::shared_ptr<PhysicsWorld> &world);

		virtual const COLLISIONTYPE getCollisionType() { return FENCETYPE; };

	private:
		btVector3 m_lastPosition;
		std::shared_ptr<PhysicsWorld> m_world;
	};
}