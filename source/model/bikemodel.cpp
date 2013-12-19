#include "bikemodel.h"
// OSG
#include <osg/BoundingBox>
// STD
#include <math.h>
// troen
#include "../constants.h"
#include "../input/bikeinputstate.h"
#include "bikemotionstate.h"

using namespace troen;

BikeModel::BikeModel(
	btTransform initialTransform,
	osg::ref_ptr<osg::Group> node,
	std::shared_ptr<FenceController> fenceController,
	BikeController* bikeController)
{
	resetState();

	osg::BoundingBox bb;
	bb.expandBy(node->getBound());

	std::shared_ptr<btBoxShape> bikeShape = std::make_shared<btBoxShape>(BIKE_DIMENSIONS / 2);

	// todo deliver "this" as a shared_ptr ?
	// jd: i tried this, this class would have to inherit from std::enable_shared_from_this<>,
	// so i thought it to complicated.
	std::shared_ptr<BikeMotionState> bikeMotionState = std::make_shared<BikeMotionState>(
		initialTransform,
		dynamic_cast<osg::PositionAttitudeTransform*> (node->getChild(0)),
		fenceController,
		this
	);
	
	btVector3 bikeInertia(0, 0, 0);
	bikeShape->calculateLocalInertia(BIKE_MASS, bikeInertia);

	btRigidBody::btRigidBodyConstructionInfo m_bikeRigidBodyCI(BIKE_MASS, bikeMotionState.get(), bikeShape.get(), bikeInertia);
	m_bikeRigidBodyCI.m_friction = 0;

	std::shared_ptr<btRigidBody> bikeRigidBody = std::make_shared<btRigidBody>(m_bikeRigidBodyCI);

	bikeRigidBody->setCcdMotionThreshold(1 / BIKE_DIMENSIONS.y());
	bikeRigidBody->setCcdSweptSphereRadius(BIKE_DIMENSIONS.x() * .5f - BIKE_DIMENSIONS.x() * 0.01);
	// this seems to be necessary so that we can move the object via setVelocity()
	bikeRigidBody->setActivationState(DISABLE_DEACTIVATION);
	bikeRigidBody->setAngularFactor(btVector3(0, 0, 1));
	// for collision event handling
	bikeRigidBody->setUserPointer(bikeController);
	bikeRigidBody->setUserIndex(BIKETYPE);

	bikeMotionState->setRigidBody(bikeRigidBody);

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

float BikeModel::getSteering() {
	return m_steering;
}

void BikeModel::updateState()
{
	const btVector3 up = btVector3(0, 0, 1);
	const btVector3 front = btVector3(0, -1, 0);

	// call this exactly once per frame
	m_steering = m_bikeInputState->getAngle();
	float velocity = m_bikeInputState->getAcceleration();

	
	std::shared_ptr<btRigidBody> bikeRigidBody = m_rigidBodies[0];
	btVector3 currentVelocityVectorXY = bikeRigidBody->getLinearVelocity();
	btScalar zComponent = currentVelocityVectorXY.getZ();
	currentVelocityVectorXY = btVector3(currentVelocityVectorXY.getX(), currentVelocityVectorXY.getY(), 0);


	// initiate rotation
	const float maximumTurn = 20;
	const float turningRad = PI / 180 * m_steering * maximumTurn;
	
	bikeRigidBody->setAngularVelocity(btVector3(0, 0, turningRad));

	// accelerate	
	const int maximumAcceleration = 5;
	// const int dampFactor = 1;
	
	int speed = currentVelocityVectorXY.length() + velocity * maximumAcceleration;

	if (speed > BIKE_MAX_VELOCITY)
		speed = BIKE_MAX_VELOCITY;

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