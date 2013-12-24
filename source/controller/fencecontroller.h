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
<<<<<<< HEAD
		FenceController(osg::Vec3 color, int maxFenceParts = 0);
		void update(btVector3 position);
		void attachWorld(std::shared_ptr<PhysicsWorld>& world);
=======
		FenceController(osg::Vec3 color, btTransform initialTransform = btTransform::getIdentity(), int maxFenceParts = 0);
		void update(btVector3 position, btQuaternion rotation);
		void attachWorld(std::weak_ptr<PhysicsWorld>& world);
>>>>>>> 15350f5ac17d9974e463e4e3e1bf2fbba0df57f6

		void removeAllFences();
		void enforceFencePartsLimit(int maxFenceParts);

	private:
		void adjustPositionUsingFenceOffset(const btQuaternion& rotation, btVector3& position);

		btVector3 m_lastPosition;
		std::weak_ptr<PhysicsWorld> m_world;
		int m_maxFenceParts;
		osg::Vec3 m_playerColor;
	};
}