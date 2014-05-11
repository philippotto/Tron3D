#include "keyboard.h"
// troen
#include "bikeinputstate.h"
#include "../view/shaders.h"
#include "../constants.h"

using namespace troen::input;

Keyboard::Keyboard(osg::ref_ptr<BikeInputState> bikeInputState, std::vector<osgGA::GUIEventAdapter::KeySymbol> keys) :
GUIEventHandler(),
PollingDevice(bikeInputState),
m_bikeInputState(bikeInputState),
m_keys(keys),
m_handbrakePressed(false)
{}

bool Keyboard::handle(const osgGA::GUIEventAdapter& eventAdapter, osgGA::GUIActionAdapter& actionAdapter)
{
	switch (eventAdapter.getEventType())
	{
	case(osgGA::GUIEventAdapter::KEYDOWN) :
	{
		int key = eventAdapter.getKey();
		if (key == m_keys[0])
		{
			m_upPressed = true;
			return false;
		}
		else if (key == m_keys[1])
		{
			m_leftPressed = true;
			return false;
		}
		else if (key == m_keys[2])
		{
			m_downPressed = true;
			return false;
		}
		else if (key == m_keys[3])
		{
			m_rightPressed = true;
			return false;
		}
		else if (key == m_keys[4])
		{
			m_handbrakePressed = 1.0;
			return false;
		}
		else if (key == m_keys[5])
		{
			m_turboPressed = true;
			return false;
		}
		return false;
	}
	case(osgGA::GUIEventAdapter::KEYUP) :
	{
		int key = eventAdapter.getKey();
		if (key == m_keys[0])
		{
			m_upPressed = false;
			return false;
		}
		else if (key == m_keys[1])
		{
			m_leftPressed = false;
			return false;
		}
		else if (key == m_keys[2])
		{
			m_downPressed = false;
			return false;
		}
		else if (key == m_keys[3])
		{
			m_rightPressed = false;
			return false;
		}
		else if (key == m_keys[4])
		{
			m_handbrakePressed = 0.0;
			return false;
		}
		else if (key == m_keys[5])
		{
			m_turboPressed = false;
			return false;
		}
		return false;
	}
	default:
		return false;
	}
}

void Keyboard::run()
{
	m_pollingEnabled = true;

	while (m_pollingEnabled)
	{
		// save angle to enable smooth direction changes
		if (m_leftPressed)
			m_angle = mi(1.0, m_angle + 0.05);
		else if (m_angle > 0)
			m_angle = 0;
		if (m_rightPressed)
			m_angle = ma(-1.0, m_angle - 0.05);
		else if (m_angle < 0)
			m_angle = 0;
		
		float leftAngle = m_leftPressed ? m_angle + m_handbrakePressed * BIKE_HANDBRAKE_FACTOR : 0.0;
		float rightAngle = m_rightPressed ? m_angle - m_handbrakePressed * BIKE_HANDBRAKE_FACTOR : 0.0;
		m_bikeInputState->setAngle(leftAngle + rightAngle);

		m_bikeInputState->setTurboPressed(m_turboPressed);

		float acceleration = m_upPressed ? 1.0 : 0.0 - m_downPressed ? -1.0 : 0.0;
		m_bikeInputState->setAcceleration(acceleration);

		this->msleep(POLLING_DELAY_MS);
	}
}