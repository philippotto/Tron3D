#include "updatebikepositioncallback.h"
// OSG
#include <osg/PositionAttitudeTransform>
// troen
#include "model/bikemodel.h"

using namespace troen;


UpdateBikePositionCallback::UpdateBikePositionCallback(std::shared_ptr<BikeModel> bike)
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

		osg::Quat rotationQuat(osg::DegreesToRadians(bikeRotation), osg::Vec3d(0.0, 0.0, 1.0));
		positionTransform->setAttitude(rotationQuat);


		btTransform trans;
		btRigidBody* bikeRigidBody = &(m_bike->getRigidBodies()->at(0));

		bikeRigidBody->getMotionState()->getWorldTransform(trans);
	
		m_bikePosition = osg::Vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());

		positionTransform->setPosition(m_bikePosition);
	}
	traverse(node, nodeVisitor);
}