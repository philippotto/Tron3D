#pragma once
#define _AMD64_

#include <osg/ref_ptr>

#include <xinput.h>

#include "../forwarddeclarations.h"

namespace troen
{
namespace input
{
	class Gamepad
	{
	public:
		Gamepad(osg::ref_ptr<BikeInputState> bikeInputState);
		int getPort();
		XINPUT_GAMEPAD* getState();
		bool checkConnection();
		bool refresh();
		bool isPressed(unsigned short button);

	private:
		osg::ref_ptr<BikeInputState> m_bikeInputState;
		int m_controllerId;
		XINPUT_STATE m_state;
		float m_deadzoneX, m_deadzoneY;
		float m_leftStickX, m_leftStickY, m_rightStickX, m_rightStickY;
		float m_leftTrigger, m_rightTrigger;
	};
}
}