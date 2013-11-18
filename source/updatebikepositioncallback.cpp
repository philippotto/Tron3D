#include "updatebikepositioncallback.h"
// OSG
#include <osg/PositionAttitudeTransform>
// troen
#include "model/bikemodel.h"

using namespace troen;


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

		btTransform trans;
		btRigidBody* bikeRigidBody = &(m_bike->getRigidBodies()->at(0));

		bikeRigidBody->getMotionState()->getWorldTransform(trans);


		
		float quadAngle = trans.getRotation().getAngle();
		btVector3 quadAxis = trans.getRotation().getAxis();

		osg::Vec3 quadAxisOSG = osg::Vec3(
			quadAxis.getX(),
			quadAxis.getY(),
			quadAxis.getZ()
		);
		
		// osg::Vec3d(0.0, 0.0, 1.0)
		osg::Quat rotationQuat(quadAngle, quadAxisOSG);
		positionTransform->setAttitude(rotationQuat);
			


		m_bikePosition = osg::Vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());

		positionTransform->setPosition(m_bikePosition);
	}
	traverse(node, nodeVisitor);
}