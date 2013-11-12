#pragma once
// troen
#include "../forwarddeclarations.h"
#include "abstractmodel.h"


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