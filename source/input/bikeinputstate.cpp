#include "bikeinputstate.h"
// troen
#include "pollingdevice.h"
#include "../constants.h"

using namespace troen::input;

BikeInputState::BikeInputState()
{
	m_acceleration = 0.0;
	m_angle = 0.0;
	m_turboPressed = false;
	m_viewingAngle = 0.0;
}

float BikeInputState::getAngle()
{
	return m_angle * BIKE_ROTATION_VALUE;
}

float BikeInputState::getAcceleration()
{
	// deceleration is stronger than acceleration
	return m_acceleration < 0 ? m_acceleration * BIKE_DECELERATION_FACTOR : m_acceleration;
}

void BikeInputState::setAngle(float angle)
{
	m_angle = angle;
}

void BikeInputState::setAcceleration(float acceleration)
{
	m_acceleration = acceleration;
}

bool BikeInputState::getTurboPressed()
{
	return m_turboPressed;
}

void BikeInputState::setTurboPressed(bool pressed)
{
	m_turboPressed = pressed;
}

float BikeInputState::getViewingAngle()
{
	return m_viewingAngle;
}

void BikeInputState::setViewingAngle(float angle)
{
	m_viewingAngle = angle;
}