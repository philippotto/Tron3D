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
		BikeInputState();
		float getAngle();
		float getAcceleration();
		void setAngle(float angle);
		void setAcceleration(float acceleration);
		void setPollingDevice(std::shared_ptr<Gamepad> pollingDevice);

	private:
		bool m_pollingEnabled;
		//TODO convert to weak_ptr
		std::shared_ptr<input::Gamepad> m_pollingDevice;
		float m_acceleration;
		float m_angle;
	};
}
}