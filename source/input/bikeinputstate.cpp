#include "bikeinputstate.h"
// troen
#include "pollingdevice.h"
#include "../constants.h"

using namespace troen::input;

BikeInputState::BikeInputState()
{
	m_acceleration = 0.0;
	m_angle = 0.0;
}

float BikeInputState::getAngle()
{
	return m_angle * BIKE_ROTATION_VALUE;
}

float BikeInputState::getAcceleration()
{
	return m_acceleration;
}

void BikeInputState::setAngle(float angle)
{
	m_angle = angle;
}

void BikeInputState::setAcceleration(float acceleration)
{
	m_acceleration = acceleration;
}