#pragma once
// bullet
#include <btBulletDynamicsCommon.h>
//troen
#include "../forwarddeclarations.h"
#include "abstractcontroller.h"

namespace troen
{
	class FenceController : public AbstractController
	{
	public:
		FenceController();
		void update(btVector3 position);

	private:
		btVector3 m_lastPosition;
	};
}