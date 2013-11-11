#include "updatebikepositioncallback.h"

#include <osg/PositionAttitudeTransform>

#include "model/bikemodel.h"


UpdateBikePositionCallback::UpdateBikePositionCallback(BikeModel* bike)
{
	m_bike = bike;

	// TODO: move to an appropriate place
	m_bikePosition = osg::Vec3d( 0.0, 0.0, 10.0 );
}

void UpdateBikePositionCallback::operator()(osg::Node* node, osg::NodeVisitor* nodeVisitor)
{
	osg::PositionAttitudeTransform* positionTransform =
		dynamic_cast<osg::PositionAttitudeTransform*> (node);

	if (positionTransform)
	{

		m_bike->updateState();
	
		float bikeRotation = m_bike->getRotation();

		// HACK remove + 180.0 due to stupid cessna looking into the wrong direction ^^
		osg::Quat rotationQuat(osg::DegreesToRadians(bikeRotation + 180.0), osg::Vec3d(0.0, 0.0, 1.0));
		positionTransform->setAttitude(rotationQuat);

		// HACK this can then also be removed
		rotationQuat.makeRotate(osg::DegreesToRadians(bikeRotation), osg::Vec3d(0.0, 0.0, 1.0));

		// apply the rotation quad to the direction vector
		// this will probably be done somewhere else as bullet needs to calculate the position
		m_bikePosition += rotationQuat * osg::Vec3d( 0.0, m_bike->getVelocity(), 0.0 );
		positionTransform->setPosition(m_bikePosition);
	}
	traverse(node, nodeVisitor);
}