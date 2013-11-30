#include "bikemodel.h"
// OSG
#include <osg/BoundingBox>
// STD
#include <math.h>
// troen
#include "../input/bikeinputstate.h"
#include "bikemotionstate.h"

using namespace troen;

#define VMAX 1000
#define FRICTION 0.1

BikeModel::BikeModel(osg::ref_ptr<osg::Group> node,
	std::shared_ptr<FenceController> fenceController,
	BikeController* bikeController)
{
	resetState();

	osg::BoundingBox bb;
	bb.expandBy(node->getBound());

#ifdef _DEBUG
	btVector3 bikeDimensions = btVector3( bb.xMax() - bb.xMin(), bb.yMax() - bb.yMin(), bb.zMax() - bb.zMin()) / 5;
#endif
#ifndef _DEBUG
	btVector3 bikeDimensions = btVector3( 12.5, 25, 12.5 );
#endif

	std::shared_ptr<btBoxShape> bikeShape = std::make_shared<btBoxShape>(bikeDimensions / 2);

	std::shared_ptr<BikeMotionState> bikeMotionState = std::make_shared<BikeMotionState>(
		btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 100)),
		dynamic_cast<osg::PositionAttitudeTransform*> (node->getChild(0)),
		fenceController,
		bikeDimensions
	);

	btScalar mass = 1000;
	btVector3 bikeInertia(0, 0, 0);
	bikeShape->calculateLocalInertia(mass, bikeInertia);

	btRigidBody::btRigidBodyConstructionInfo m_bikeRigidBodyCI(mass, bikeMotionState.get(), bikeShape.get(), bikeInertia);

	std::shared_ptr<btRigidBody> bikeRigidBody = std::make_shared<btRigidBody>(m_bikeRigidBodyCI);

	bikeRigidBody->setCcdMotionThreshold(1 / bikeDimensions.y());
	bikeRigidBody->setLinearVelocity(btVector3(1, 0, 1));
	// this seems to be necessary so that we can move the object via setVelocity()
	bikeRigidBody->setActivationState(DISABLE_DEACTIVATION);
	bikeRigidBody->setAngularFactor(btVector3(0, 0, 1));
	// for collision event handling
	bikeRigidBody->setUserPointer(bikeController);

	m_collisionShapes.push_back(bikeShape);
	m_motionStates.push_back(bikeMotionState);
	m_rigidBodies.push_back(bikeRigidBody);
}

void BikeModel::setInputState(osg::ref_ptr<input::BikeInputState> bikeInputState)
{
	m_bikeInputState = bikeInputState;
}

void BikeModel::resetState()
{
	m_velocity = 0.0;
	m_rotation = 0.0;
}

void BikeModel::updateState()
{
	const btVector3 up = btVector3(0, 0, 1);
	const btVector3 front = btVector3(0, -1, 0);

	// call this exactly once per frame
	float angle = m_bikeInputState->getAngle();
	float velocity = m_bikeInputState->getAcceleration();

	
	std::shared_ptr<btRigidBody> bikeRigidBody = m_rigidBodies[0];
	btVector3 currentVelocityVectorXY = bikeRigidBody->getLinearVelocity();
	btScalar zComponent = currentVelocityVectorXY.getZ();
	currentVelocityVectorXY = btVector3(currentVelocityVectorXY.getX(), currentVelocityVectorXY.getY(), 0);


	// initiate rotation
	const float maximumTurn = 20;
	const float turningRad = 3.14 / 180 * angle * maximumTurn;
	
	
	bikeRigidBody->setAngularVelocity(btVector3(0, 0, turningRad));

	// accelerate	
	const int maximumAcceleration = 5;
	// const int dampFactor = 1;
	
	int speed = currentVelocityVectorXY.length() + velocity * maximumAcceleration;

	if (speed > VMAX)
		speed = VMAX;

	// adapt velocity vector to real direction

	float quat =  bikeRigidBody->getOrientation().getAngle();
	btVector3 axis = bikeRigidBody->getOrientation().getAxis();
	
	currentVelocityVectorXY = front.rotate(axis, quat) * speed;
	

	currentVelocityVectorXY.setZ(zComponent);
	bikeRigidBody->setLinearVelocity(currentVelocityVectorXY);

}

float BikeModel::getRotation()
{
	return m_rotation;
}

float BikeModel::getVelocity()
{
	return m_velocity;
}

osg::Vec3d BikeModel::getPositionOSG()
{
	btTransform trans;
	(m_rigidBodies[0]->getMotionState()->getWorldTransform(trans));

	return osg::Vec3d(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
}

btVector3 BikeModel::getPositionBt()
{
	btTransform trans;
	(m_rigidBodies[0]->getMotionState()->getWorldTransform(trans));

	return trans.getOrigin();
}