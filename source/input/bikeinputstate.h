#pragma once

#include <osg/Referenced>

#include "../forwarddeclarations.h"

namespace troen
{
namespace input
{
	class BikeInputState : public osg::Referenced
	{
	public:
		BikeInputState() : m_pollingEnabled(false), m_pollingDevice(NULL) {}
		BikeInputState(bool pollingEnabled, std::shared_ptr<input::Gamepad> pollingDevice);
		float getAngle();
		float getAcceleration();
		void setAngle(float angle);
		void setAcceleration(float acceleration);

	private:
		bool m_pollingEnabled;
		std::shared_ptr<input::Gamepad> m_pollingDevice;
		float m_acceleration;
		float m_angle;
	};
}
}