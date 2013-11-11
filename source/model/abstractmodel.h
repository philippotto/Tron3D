#pragma once

#include <vector>
#include "../forwarddeclarations.h"

#include <btBulletDynamicsCommon.h>

namespace troen
{
	class AbstractModel
	{
		public:
			AbstractModel();
		
		protected:
			virtual std::vector<btRigidBody> getRigidBodies() = 0;

		

	};
}