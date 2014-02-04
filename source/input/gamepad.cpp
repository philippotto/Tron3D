#ifdef WIN32

#include "gamepad.h"
// troen
#include "bikeinputstate.h"
#include "../constants.h"
// other
#include <WinBase.h>
#include <numeric>

using namespace troen::input;

std::vector <int> Gamepad::freePorts(XUSER_MAX_COUNT);

std::vector <int>* Gamepad::getFreePorts()
{
	// initialize freePorts
	if (Gamepad::freePorts.at(XUSER_MAX_COUNT - 1) == 0){
		std::iota(Gamepad::freePorts.begin(), Gamepad::freePorts.end(), 0);
	}
	return &Gamepad::freePorts;
}

Gamepad::~Gamepad() {
	if (m_controllerId >= 0) {
		setVibration(false);
	}
}

XINPUT_GAMEPAD* Gamepad::getState()
{
	return &m_state.Gamepad;
}

int Gamepad::getPort()
{
	return m_controllerId;
}

bool Gamepad::checkConnection()
{
	std::vector <int> freePorts = *Gamepad::getFreePorts();
	m_isConnected = false;

	if (m_controllerId == -1) {
		for (auto i : freePorts)
		{
			ZeroMemory(&m_state, sizeof(XINPUT_STATE));

			if (XInputGetState(i, &m_state) == ERROR_SUCCESS) {
				m_controllerId = i;
				m_isConnected = true;
				// remove element with value i from vector
				freePorts.erase(std::remove(freePorts.begin(), freePorts.end(), i), freePorts.end());
				break;
			}
		}
	}
	else {
		ZeroMemory(&m_state, sizeof(XINPUT_STATE));

		if (XInputGetState(m_controllerId, &m_state) == ERROR_SUCCESS) {
			m_isConnected = true;
		}
	}

	return m_isConnected;
}

// Returns false if the controller has been disconnected
void Gamepad::run()
{
	m_pollingEnabled = true;

	while (m_pollingEnabled)
	{

		if (!m_isConnected)
			checkConnection();
	
		if (m_isConnected)
		{
			ZeroMemory(&m_state, sizeof(XINPUT_STATE));
			if (XInputGetState(m_controllerId, &m_state) != ERROR_SUCCESS)
			{
				m_bikeInputState->setAngle(0);
				m_bikeInputState->setAcceleration(0);
				m_bikeInputState->setTurboPressed(false);
				m_bikeInputState->setViewingAngle(0);
				m_isConnected = false;
				continue;
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

			float viewingAngle;
			if (m_rightStickX != 0.0 || m_rightStickY != 0.0) {
				float relativeAngle = atan(m_rightStickX / m_rightStickY);// *abs(m_rightStickX);
				viewingAngle = (m_rightStickY < 0.f ? m_rightStickX < 0 ? -PI + relativeAngle : PI + relativeAngle : relativeAngle);
			}
			else {
				viewingAngle = 0.f;
			}

			m_bikeInputState->setAcceleration(m_rightTrigger - m_leftTrigger);
			m_bikeInputState->setAngle(-m_leftStickX - m_leftStickX * handbrakePressed * BIKE_HANDBRAKE_FACTOR);
			m_bikeInputState->setTurboPressed(turboPressed);
			m_bikeInputState->setViewingAngle(-viewingAngle);

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