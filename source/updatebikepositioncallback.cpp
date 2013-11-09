#include "updatebikepositioncallback.h"

#include <osg/PositionAttitudeTransform>

#include "BikeInputState.h"

UpdateBikePositionCallback::UpdateBikePositionCallback(BikeInputState* bikeInputState)
{
	m_bikeInputState = bikeInputState;
	m_rotation = 0.0;
	m_bikePosition = osg::Vec3d({ 0.0, 0.0, 0.0 });
}

void UpdateBikePositionCallback::operator()(osg::Node* node, osg::NodeVisitor* nodeVisitor)
{
	osg::PositionAttitudeTransform* positionTransform =
		dynamic_cast<osg::PositionAttitudeTransform*> (node);
	if (positionTransform)
	{
		if (m_bikeInputState->m_direction.length())
		{
			m_bikePosition += osg::Vec3d({ m_bikeInputState->m_direction.x(), m_bikeInputState->m_direction.y(), 0 });
			positionTransform->setPosition(m_bikePosition);
		}
	}
	traverse(node, nodeVisitor);
}