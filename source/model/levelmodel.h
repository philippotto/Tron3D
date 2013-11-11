#pragma once

#include "abstractmodel.h"
#include "../forwarddeclarations.h"
#include <memory>

namespace troen
{
	class LevelModel : public AbstractModel
	{
	public:
		LevelModel();
		std::shared_ptr<std::vector<btRigidBody>> getRigidBodies();
	
	private:
		std::shared_ptr<std::vector<btRigidBody>> m_rigidBodies;
	
	protected:
		
	};
}