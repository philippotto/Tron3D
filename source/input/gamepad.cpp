#include "gamepad.h"
// STD
#include <iostream>
// troen
#include "bikeinputstate.h"
// other
#include <WinBase.h>

using namespace troen::input;

Gamepad::Gamepad(osg::ref_ptr<BikeInputState> bikeInputState)
{
	m_bikeInputState = bikeInputState;
	m_deadzoneX = 0.05f;
	m_deadzoneY = 0.02f;
}

int Gamepad::getPort()
{
	return m_controllerId + 1;
}

XINPUT_GAMEPAD* Gamepad::getState()
{
	return &m_state.Gamepad;
}

bool Gamepad::checkConnection()
{
	int cId = -1;

	for (int i = 0; i < XUSER_MAX_COUNT && cId == -1; i++)
	{
		ZeroMemory(&m_state, sizeof(XINPUT_STATE));

		if (XInputGetState(i, &m_state) == ERROR_SUCCESS)
			cId = i;
	}

	m_controllerId = cId;

	return m_controllerId != -1;
}

// Returns false if the controller has been disconnected
bool Gamepad::refresh()
{
	if (m_controllerId == -1)
		checkConnection();
	
	if (m_controllerId != -1)
	{
		ZeroMemory(&m_state, sizeof(XINPUT_STATE));
		if (XInputGetState(m_controllerId, &m_state) != ERROR_SUCCESS)
		{
			m_controllerId = -1;
			return false;
		}

		float normLX = fmaxf(-1, (float) m_state.Gamepad.sThumbLX / 32767);
		float normLY = fmaxf(-1, (float) m_state.Gamepad.sThumbLY / 32767);

		m_leftStickX = (abs(normLX) < m_deadzoneX ? 0 : (abs(normLX) - m_deadzoneX) * (normLX / abs(normLX)));
		m_leftStickY = (abs(normLY) < m_deadzoneY ? 0 : (abs(normLY) - m_deadzoneY) * (normLY / abs(normLY)));

		if (m_deadzoneX > 0) m_leftStickX *= 1 / (1 - m_deadzoneX);
		if (m_deadzoneY > 0) m_leftStickY *= 1 / (1 - m_deadzoneY);

		float normRX = fmaxf(-1, (float) m_state.Gamepad.sThumbRX / 32767);
		float normRY = fmaxf(-1, (float) m_state.Gamepad.sThumbRY / 32767);

		m_rightStickX = (abs(normRX) < m_deadzoneX ? 0 : (abs(normRX) - m_deadzoneX) * (normRX / abs(normRX)));
		m_rightStickY = (abs(normRY) < m_deadzoneY ? 0 : (abs(normRY) - m_deadzoneY) * (normRY / abs(normRY)));

		if (m_deadzoneX > 0) m_rightStickX *= 1 / (1 - m_deadzoneX);
		if (m_deadzoneY > 0) m_rightStickY *= 1 / (1 - m_deadzoneY);

		m_leftTrigger = (float) m_state.Gamepad.bLeftTrigger / 255;
		m_rightTrigger = (float) m_state.Gamepad.bRightTrigger / 255;

		m_bikeInputState->setAcceleration(m_rightTrigger - m_leftTrigger);
		m_bikeInputState->setAngle(-m_leftStickX);

		return true;
	}
	return false;
}

bool Gamepad::isPressed(unsigned short button)
{
	return (m_state.Gamepad.wButtons & button) != 0;
}