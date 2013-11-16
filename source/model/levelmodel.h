#pragma once
// STD
#include <memory>
//troen
#include "../forwarddeclarations.h"
#include "abstractmodel.h"

namespace troen
{
	class LevelModel : public AbstractModel
	{
	public:
		LevelModel();
		std::shared_ptr<std::vector<btRigidBody>> getRigidBodies();
		int getLevelSize();
	private:
		void addWalls();
		std::shared_ptr<std::vector<btRigidBody>> m_rigidBodies;
		
	
	protected:
		
	};
}