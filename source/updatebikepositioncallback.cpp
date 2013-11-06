#include "updatebikepositioncallback.h"

#include <osg/PositionAttitudeTransform>

#include "BikeInputState.h"

void UpdateBikePositionCallback::operator()(osg::Node* node, osg::NodeVisitor* nodeVisitor)
{
	osg::PositionAttitudeTransform* positionTransform =
		dynamic_cast<osg::PositionAttitudeTransform*> (node);
	if (positionTransform)
	{
		if (m_bikeInputState->direction.length())
		{
			m_bikePosition.set(m_bikeInputState->direction.x(), m_bikeInputState->direction.y(), 0);
			positionTransform->setPosition(m_bikePosition);
		}
	}
	traverse(node, nodeVisitor);
}