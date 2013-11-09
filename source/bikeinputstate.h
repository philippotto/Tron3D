#pragma once

#include <osg/ref_ptr>
#include <osg/Vec2d>

class BikeInputState
{
public:
	BikeInputState();
	void resetState();
	osg::Vec2d m_direction;
};