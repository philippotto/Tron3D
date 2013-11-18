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
		int getLevelSize();

	private:
		void addWalls();	
	};
}