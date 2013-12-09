#pragma once
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
		Gamepad(osg::ref_ptr<BikeInputState> bikeInputState) : PollingDevice(bikeInputState) {};
		int getPort();
		XINPUT_GAMEPAD* getState();
		bool checkConnection();
		bool refresh() override;
		bool isPressed(unsigned short button);

	private:
		int m_controllerId;
		XINPUT_STATE m_state;
		float m_deadzoneX, m_deadzoneY;
		float m_leftStickX, m_leftStickY, m_rightStickX, m_rightStickY;
		float m_leftTrigger, m_rightTrigger;
	};
}
}