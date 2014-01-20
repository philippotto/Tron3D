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
		osg::ref_ptr<osg::Group>  getFloorView();

	private:
		std::vector<btTransform> m_initialBikePositionTransforms;
		virtual void initializeinitialBikePositionTransforms();
	};
}