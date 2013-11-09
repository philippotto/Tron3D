#include "updatebikepositioncallback.h"

#include <osg/PositionAttitudeTransform>

#include "input/bikeinputstate.h"

UpdateBikePositionCallback::UpdateBikePositionCallback(BikeInputState* bikeInputState)
{
	m_bikeInputState = bikeInputState;
	m_bikeRotation = 0.0;
	m_bikePosition = osg::Vec3d({ 0.0, 0.0, 0.0 });
}

void UpdateBikePositionCallback::operator()(osg::Node* node, osg::NodeVisitor* nodeVisitor)
{
	osg::PositionAttitudeTransform* positionTransform =
		dynamic_cast<osg::PositionAttitudeTransform*> (node);
	
	// reset rotation if it's more than 360°
	m_bikeRotation = fmod(m_bikeRotation + m_bikeInputState->m_rotation, 360.0);

	// HACK remove + 90.0 due to stupid cow looking into the wrong direction ^^
	osg::Quat rotationQuat(osg::DegreesToRadians(m_bikeRotation + 90.0), osg::Vec3d(0.0, 0.0, 1.0));
	positionTransform->setAttitude(rotationQuat);

	// HACK this can then also be removed
	rotationQuat.makeRotate(osg::DegreesToRadians(m_bikeRotation), osg::Vec3d(0.0, 0.0, 1.0));

	if (positionTransform)
	{
		if (m_bikeInputState->m_direction.length())
		{
			// apply the rotation quad to the direction vector
			m_bikePosition += rotationQuat * osg::Vec3d({ m_bikeInputState->m_direction.x(), m_bikeInputState->m_direction.y(), 0 });
			positionTransform->setPosition(m_bikePosition);
		}

	}
	traverse(node, nodeVisitor);
}