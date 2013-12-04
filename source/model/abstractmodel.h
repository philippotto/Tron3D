#pragma once
// STD
#include <vector>
#include <deque>
// bullet
#include <btBulletDynamicsCommon.h>
// troen
#include "../forwarddeclarations.h"

namespace troen
{
	class AbstractModel
	{
		public:
			AbstractModel();
		
			virtual std::vector<std::shared_ptr<btRigidBody>> getRigidBodies();
		protected:
			std::vector<std::shared_ptr<btRigidBody>> m_rigidBodies;
			std::vector<std::shared_ptr<btMotionState>> m_motionStates;
			std::vector<std::shared_ptr<btCollisionShape>> m_collisionShapes;
	};
}