#include "bikemodel.h"
// STD
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

// TODO
// this is where physics/bullet should kick in and adjust these values

void BikeModel::rotate(float angle)
{
	// reset rotation if it's more than 360°
	m_rotation = fmod(m_rotation + angle, 360.f);
}

void BikeModel::accelerate(float velocity)
{
	m_velocity = fmax(fmin(m_velocity + velocity - FRICTION, VMAX), 0.f);
}

float BikeModel::getRotation()
{
	return m_rotation;
}

float BikeModel::getVelocity()
{
	return m_velocity;
}


std::vector<btRigidBody> BikeModel::getRigidBodies(){
	std::vector<btRigidBody> a;


	return a;
}