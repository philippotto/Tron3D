#pragma once
// OSG
#include <osg/Referenced>
// troen
#include "../forwarddeclarations.h"

namespace troen
{
namespace input
{
	class BikeInputState : public osg::Referenced
	{
	public:
		enum InputDevice
		{
			KEYBOARD_wasd, KEYBOARD_arrows, GAMEPAD, GAMEPADPS4, AI
		};

		BikeInputState();
		float getAngle();
		float getAcceleration();
		void setAngle(float angle);
		void setAcceleration(float acceleration);

	private:
		float m_acceleration;
		float m_angle;
	};
}
}