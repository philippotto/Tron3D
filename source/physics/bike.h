#pragma once

#include <osg/Referenced>

#include "../forwarddeclarations.h"

using namespace troen::input;

namespace troen
{
namespace physics
{
	class Bike : public osg::Referenced
	{
	public:
		Bike(BikeInputState* bikeInputState);
		void resetState();
		void updateState();
		void rotate(float angle);
		void accelerate(float velocity);
		float getRotation();
		float getVelocity();

	private:
		BikeInputState* m_bikeInputState;
		float m_velocity;
		float m_rotation;
	};
}
}