#include "bikemodel.h"
// STD
#include <iostream>
#include <math.h>
// troen
#include "../input/bikeinputstate.h"

using namespace troen;

#define VMAX 10.0
#define FRICTION 0.1

BikeModel::BikeModel(osg::ref_ptr<input::BikeInputState> bikeInputState)
{
	m_bikeInputState = bikeInputState;
	resetState();


	m_rigidBodies = std::make_shared<std::vector<btRigidBody>>();

	// radius: 1 meter
	// TODO adjust to bounding box of bike
	btBoxShape *boxShape = new btBoxShape(btVector3(13.5, 23, 10));

	// TODO: convert to shared_ptr
	btDefaultMotionState *bikeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 100)));
	btScalar mass = 1;
	btVector3 bikeInertia(0, 0, 0);
	boxShape->calculateLocalInertia(mass, bikeInertia);

	btRigidBody::btRigidBodyConstructionInfo m_bikeRigidBodyCI(mass, bikeMotionState, boxShape, bikeInertia);

	btRigidBody bikeRigidBody(m_bikeRigidBodyCI);


	bikeRigidBody.setLinearVelocity(btVector3(1, 0, 1));

	// this seems to be necessary so that we can move the object via setVelocity()
	bikeRigidBody.setActivationState(DISABLE_DEACTIVATION);

	m_rigidBodies->push_back(bikeRigidBody);
}

void BikeModel::resetState()
{
	m_velocity = 0.0;
	m_rotation = 0.0;
}

void BikeModel::updateState()
{
	// call this exactly once per frame
	rotate(m_bikeInputState->getAngle());
	accelerate(m_bikeInputState->getAcceleration());
}

void BikeModel::rotate(float angle)
{
	// reset rotation if it's more than 360°
	m_rotation = fmod(m_rotation + angle, 360.f);
}

void BikeModel::accelerate(float velocity)
{
	m_velocity = fmax(fmin(m_velocity + velocity - FRICTION, VMAX), 0.f);

	btRigidBody* bikeRigidBody = &(m_rigidBodies->at(0));

	btScalar rotationScalar((m_rotation - 90) * 3.14 / 180);

	btVector3 currentVelocityVector = bikeRigidBody->getLinearVelocity();
	
	const btVector3 velocityVector = btVector3(10 * m_velocity, 0, 0).rotate(btVector3(0, 0, 1), rotationScalar);

	currentVelocityVector.setX(velocityVector.x());
	currentVelocityVector.setY(velocityVector.y());

	bikeRigidBody->setLinearVelocity(currentVelocityVector);

}

float BikeModel::getRotation()
{
	return m_rotation;
}

float BikeModel::getVelocity()
{
	return m_velocity;
}


std::shared_ptr<std::vector<btRigidBody>> BikeModel::getRigidBodies()
{
	return m_rigidBodies;
}