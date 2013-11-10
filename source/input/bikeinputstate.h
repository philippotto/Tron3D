#pragma once

#include <osg/Referenced>

namespace troen
{
namespace input
{
	class BikeInputState : public osg::Referenced
	{
	public:
		BikeInputState();
		float m_acceleration;
		float m_angle;
	};
}
}