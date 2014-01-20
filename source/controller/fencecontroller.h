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
		FenceController(BikeController* bikeController, osg::Vec3 color, btTransform initialTransform = btTransform::getIdentity());
		void update(btVector3 position, btQuaternion rotation);
		void attachWorld(std::shared_ptr<PhysicsWorld>& world);

		void removeAllFences();
		int getFenceLimit();

		void setLastPosition(const btQuaternion rotation, btVector3 position);

	private:
		void adjustPositionUsingFenceOffset(const btQuaternion& rotation, btVector3& position);

		btVector3 m_lastPosition;
		std::weak_ptr<PhysicsWorld> m_world;
		osg::Vec3 m_playerColor;
		BikeController* m_bikeController;
	};
}