#pragma once

#include <vector>
#include "../forwarddeclarations.h"

#include <btBulletDynamicsCommon.h>
#include <memory>

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