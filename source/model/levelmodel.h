#pragma once

#include "abstractmodel.h"
#include "../forwarddeclarations.h"

namespace troen
{
	class LevelModel : public AbstractModel
	{
	public:
		LevelModel();

	protected:
		std::vector<btRigidBody> getRigidBodies();
	};
}