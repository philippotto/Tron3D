#include "bikeinputstate.h"

#include "gamepad.h"

using namespace troen::input;

#define MOVE_VALUE 2.0
#define ROTATION_VALUE 3.0

BikeInputState::BikeInputState(bool pollingEnabled, std::shared_ptr<Gamepad> pollingDevice)
{
	m_pollingEnabled = pollingEnabled;
	m_acceleration = 0.0;
	m_angle = 0.0;
}

float BikeInputState::getAngle()
{
	if (m_pollingEnabled)
	{
		m_pollingDevice->refresh();
	}

	return m_angle * ROTATION_VALUE;
}

float BikeInputState::getAcceleration()
{
	// TODO dw: don't refresh twice
	if (m_pollingEnabled)
	{
		m_pollingDevice->refresh();
	}

	return m_acceleration * MOVE_VALUE;
}

void BikeInputState::setAngle(float angle)
{
	m_angle = angle;
}

void BikeInputState::setAcceleration(float acceleration)
{
	m_acceleration = acceleration;
}