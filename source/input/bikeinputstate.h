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
			KEYBOARD_wasd, KEYBOARD_arrows, GAMEPAD, AI
		};

		BikeInputState();
		float getAngle();
		float getAcceleration();
		void setAngle(float angle);
		void setAcceleration(float acceleration);
		void setPollingDevice(std::shared_ptr<PollingDevice> pollingDevice);

	private:
		bool m_pollingEnabled;
		std::shared_ptr<input::PollingDevice> m_pollingDevice;
		float m_acceleration;
		float m_angle;
	};
}
}