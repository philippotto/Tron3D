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
		BikeInputState();
		float getAngle();
		float getAcceleration();
		void setAngle(float angle);
		void setAcceleration(float acceleration);
		void setPollingDevice(std::shared_ptr<Gamepad> pollingDevice);

	private:
		bool m_pollingEnabled;
		std::shared_ptr<input::Gamepad> m_pollingDevice;
		float m_acceleration;
		float m_angle;
	};
}
}