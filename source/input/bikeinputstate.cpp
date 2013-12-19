#include "bikeinputstate.h"
// troen
#include "../constants.h"
#include "gamepad.h"

using namespace troen::input;

BikeInputState::BikeInputState()
{
	m_pollingEnabled = false;
	m_acceleration = 0.0;
	m_angle = 0.0;
}

void BikeInputState::setPollingDevice(std::shared_ptr<PollingDevice> pollingDevice)
{
	m_pollingEnabled = true;
	m_pollingDevice = pollingDevice;
}

float BikeInputState::getAngle()
{
	if (m_pollingEnabled)
	{
		m_pollingDevice->refresh();
	}

	return m_angle * BIKE_ROTATION_VALUE;
}

float BikeInputState::getAcceleration()
{
	// TODO dw: don't refresh twice
	if (m_pollingEnabled)
	{
		m_pollingDevice->refresh();
	}

	return m_acceleration * BIKE_MOVE_VALUE;
}

void BikeInputState::setAngle(float angle)
{
	m_angle = angle;
}

void BikeInputState::setAcceleration(float acceleration)
{
	m_acceleration = acceleration;
}