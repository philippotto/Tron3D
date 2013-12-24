#include "bikemodel.h"
// OSG
#include <osg/BoundingBox>
// STD
#include <math.h>
// troen
#include "../input/bikeinputstate.h"
#include "bikemotionstate.h"

using namespace troen;

#define VMAX 800
#define FRICTION 10
#define PI 3.14159265359

BikeModel::BikeModel(osg::ref_ptr<osg::Group> node,
	std::shared_ptr<FenceController> fenceController,
	BikeController* bikeController)
{
	resetState();

	osg::BoundingBox bb;
	bb.expandBy(node->getBound());

	btVector3 bikeDimensions = btVector3( 12.5, 25, 12.5 );

	std::shared_ptr<btBoxShape> bikeShape = std::make_shared<btBoxShape>(bikeDimensions / 2);

	// todo deliver "this" as a shared_ptr ?
	std::shared_ptr<BikeMotionState> bikeMotionState = std::make_shared<BikeMotionState>(
		btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, bikeDimensions.z()/2)),
		dynamic_cast<osg::PositionAttitudeTransform*> (node->getChild(0)),
		fenceController,
		this,
		bikeDimensions
	);
	
	btScalar mass = 10;
	btVector3 bikeInertia(0, 0, 0);
	bikeShape->calculateLocalInertia(mass, bikeInertia);

	btRigidBody::btRigidBodyConstructionInfo m_bikeRigidBodyCI(mass, bikeMotionState.get(), bikeShape.get(), bikeInertia);
	m_bikeRigidBodyCI.m_friction = 0;

	std::shared_ptr<btRigidBody> bikeRigidBody = std::make_shared<btRigidBody>(m_bikeRigidBodyCI);

	bikeRigidBody->setCcdMotionThreshold(1 / bikeDimensions.y());
	bikeRigidBody->setCcdSweptSphereRadius(bikeDimensions.x() / 2.0 - 0.5);
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
	//const btVector3 up = btVector3(0, 0, 1);
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