#pragma once
// OSG
#include <osg/ref_ptr>
// troen
#include "abstractcontroller.h"
#include "../forwarddeclarations.h"

namespace troen
{
	class LevelController : public AbstractController
	{
	public:
		LevelController();

		virtual btTransform initialPositionTransformForBikeWithIndex(int index);
		void attachWorld(std::shared_ptr<PhysicsWorld> &world);
		void addItemBox(btVector3 &position);

	private:
		std::vector<btTransform> m_initialBikePositionTransforms;
		virtual void initializeinitialBikePositionTransforms();
		std::weak_ptr<PhysicsWorld> m_world;
	};
}