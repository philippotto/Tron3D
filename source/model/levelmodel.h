#pragma once
//troen
#include "../forwarddeclarations.h"
#include "abstractmodel.h"

namespace troen
{
	class LevelModel : public AbstractModel
	{
	public:
		LevelModel(const LevelController* levelController);
		int getLevelSize();

	private:
		void addWalls();	
	};
}