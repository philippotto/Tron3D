#pragma once
// STD
#include <vector>
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
		
		protected:
			virtual std::vector<btRigidBody> getRigidBodies() = 0;

	};
}