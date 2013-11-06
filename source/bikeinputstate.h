#pragma once

#include <osg/ref_ptr>
#include <osg/Vec2>

class BikeInputState
{
public:
	BikeInputState::BikeInputState() :
		direction(osg::Vec2d({ 0, 1 })) {}
	osg::Vec2d direction;
};