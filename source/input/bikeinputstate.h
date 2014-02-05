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
		void setAngle(const float angle);
		void setAcceleration(const float acceleration);
		void setTurboPressed(const bool pressed);
		bool getTurboPressed();
		void setViewingAngle(float angle);
		float getViewingAngle();
	private:
		float m_acceleration;
		float m_angle;
		bool m_turboPressed;
		float m_viewingAngle;
	};
}
}