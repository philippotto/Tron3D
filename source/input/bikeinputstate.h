#pragma once

#include <osg/Referenced>
#include <osg/Vec2d>

class BikeInputState : public osg::Referenced
{
public:
	BikeInputState();
	void resetState();
	osg::Vec2d m_direction;
	float m_rotation;
};