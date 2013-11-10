#include "bike.h"

#include <math.h>

#include "../input/bikeinputstate.h"

using namespace troen::physics;

#define VMAX 10.0
#define FRICTION 0.1

Bike::Bike(BikeInputState* bikeInputState)
{
	m_bikeInputState = bikeInputState;
	resetState();
}

void Bike::resetState()
{
	m_velocity = 0.0;
	m_rotation = 0.0;
}

void Bike::updateState()
{
	// call this exactly once per frame
	rotate(m_bikeInputState->m_angle);
	accelerate(m_bikeInputState->m_acceleration);
}

// TODO
// this is where physics/bullet should kick in and adjust these values

void Bike::rotate(float angle)
{
	// reset rotation if it's more than 360°
	m_rotation = fmod(m_rotation + angle, 360.f);
}

void Bike::accelerate(float velocity)
{
	m_velocity = fmax(fmin(m_velocity + velocity - FRICTION, VMAX), 0.f);
}

float Bike::getRotation()
{
	return m_rotation;
}

float Bike::getVelocity()
{
	return m_velocity;
}