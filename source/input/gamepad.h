#pragma once

#ifdef WIN32

// OSG
#include <osg/ref_ptr>
// troen
#include "pollingdevice.h"
#include "../forwarddeclarations.h"
//other
#define _AMD64_
#include <xinput.h>

namespace troen
{
namespace input
{
	class Gamepad : public PollingDevice
	{
	public:
		Gamepad(osg::ref_ptr<BikeInputState> bikeInputState) : PollingDevice(bikeInputState) {
			m_deadzoneX = 0.25f;
			m_deadzoneY = 0.02f;
		};
		~Gamepad();
		int getPort();
		XINPUT_GAMEPAD* getState();
		bool checkConnection();
		void run() override;
		bool isPressed(unsigned short button);

		void vibrate();

	private:
		int m_controllerId;
		XINPUT_STATE m_state;
		float m_deadzoneX, m_deadzoneY;
		float m_leftStickX, m_leftStickY, m_rightStickX, m_rightStickY;
		float m_leftTrigger, m_rightTrigger;
	};
}
}

#endif