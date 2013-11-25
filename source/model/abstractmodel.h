#pragma once
// STD
#include <vector>
#include <memory>
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
	};
}