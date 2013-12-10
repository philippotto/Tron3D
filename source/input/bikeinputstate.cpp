#include "bikeinputstate.h"
// troen
#include "gamepad.h"

using namespace troen::input;

#define MOVE_VALUE 2.0
#define ROTATION_VALUE 5.0

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
		m_pollingDevice.lock()->refresh();
	}

	return m_angle * ROTATION_VALUE;
}

float BikeInputState::getAcceleration()
{
	// TODO dw: don't refresh twice
	if (m_pollingEnabled)
	{
		m_pollingDevice.lock()->refresh();
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