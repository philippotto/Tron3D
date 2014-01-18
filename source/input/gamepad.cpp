#ifdef WIN32

#include "gamepad.h"
// troen
#include "bikeinputstate.h"
#include "../constants.h"
// other
#include <WinBase.h>

using namespace troen::input;

int Gamepad::getPort()
{
	return m_controllerId + 1;
}

Gamepad::~Gamepad() {
	if (m_controllerId > 0) {
		setVibration(false);
	}
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
void Gamepad::run()
{
	m_pollingEnabled = true;

	while (m_pollingEnabled)
	{

		if (m_controllerId == -1)
			checkConnection();
	
		if (m_controllerId != -1)
		{
			ZeroMemory(&m_state, sizeof(XINPUT_STATE));
			if (XInputGetState(m_controllerId, &m_state) != ERROR_SUCCESS)
			{
				m_bikeInputState->setAngle(0);
				m_bikeInputState->setAcceleration(0);
				m_controllerId = -1;
				return;
			}

			float normLX = fmaxf(-1, (float)m_state.Gamepad.sThumbLX / 32767);
			float normLY = fmaxf(-1, (float)m_state.Gamepad.sThumbLY / 32767);

			m_leftStickX = (abs(normLX) < m_deadzoneX ? 0 : (abs(normLX) - m_deadzoneX) * (normLX / abs(normLX)));
			m_leftStickY = (abs(normLY) < m_deadzoneY ? 0 : (abs(normLY) - m_deadzoneY) * (normLY / abs(normLY)));

			if (m_deadzoneX > 0) m_leftStickX *= 1 / (1 - m_deadzoneX);
			if (m_deadzoneY > 0) m_leftStickY *= 1 / (1 - m_deadzoneY);

			float normRX = fmaxf(-1, (float)m_state.Gamepad.sThumbRX / 32767);
			float normRY = fmaxf(-1, (float)m_state.Gamepad.sThumbRY / 32767);

			m_rightStickX = (abs(normRX) < m_deadzoneX ? 0 : (abs(normRX) - m_deadzoneX) * (normRX / abs(normRX)));
			m_rightStickY = (abs(normRY) < m_deadzoneY ? 0 : (abs(normRY) - m_deadzoneY) * (normRY / abs(normRY)));

			if (m_deadzoneX > 0) m_rightStickX *= 1 / (1 - m_deadzoneX);
			if (m_deadzoneY > 0) m_rightStickY *= 1 / (1 - m_deadzoneY);

			m_leftTrigger = (float)m_state.Gamepad.bLeftTrigger / 255;
			m_rightTrigger = (float)m_state.Gamepad.bRightTrigger / 255;

			float handbrakePressed = isPressed(XINPUT_GAMEPAD_X);
			bool turboPressed = isPressed(XINPUT_GAMEPAD_A);

			m_bikeInputState->setAcceleration(m_rightTrigger - m_leftTrigger);
			m_bikeInputState->setAngle(-m_leftStickX - m_leftStickX * handbrakePressed * BIKE_HANDBRAKE_FACTOR);
			m_bikeInputState->setTurboPressed(turboPressed);

			vibrate();

		}

		this->msleep(POLLING_DELAY_MS);
	}
}


void Gamepad::vibrate() {
	bool b = m_vibrationEnabled;

	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

	vibration.wLeftMotorSpeed = b ? 65535 : 0; // use any value between 0-65535 here
	vibration.wRightMotorSpeed = b ? 32000 : 0; // use any value between 0-65535 here
	XInputSetState(m_controllerId, &vibration);
}

bool Gamepad::isPressed(unsigned short button)
{
	return (m_state.Gamepad.wButtons & button) != 0;
}


#endif