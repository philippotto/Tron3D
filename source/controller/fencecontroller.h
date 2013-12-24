#pragma once
// bullet
#include <btBulletDynamicsCommon.h>
//troen
#include "../forwarddeclarations.h"
#include "abstractcontroller.h"

#include <osg/Vec3>

namespace troen
{
	class FenceController : public AbstractController
	{
	public:
		FenceController(osg::Vec3 color, int maxFenceParts = 0);
		void update(btVector3 position);
		void attachWorld(std::shared_ptr<PhysicsWorld>& world);

		void removeAllFences();
		void enforceFencePartsLimit(int maxFenceParts);

	private:
		btVector3 m_lastPosition;
		std::weak_ptr<PhysicsWorld> m_world;
		int m_maxFenceParts;
		osg::Vec3 m_playerColor;
	};
}