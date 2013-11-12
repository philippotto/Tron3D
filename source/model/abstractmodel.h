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
		
					virtual std::shared_ptr<std::vector<btRigidBody>> getRigidBodies() = 0;
		protected:

	};
}