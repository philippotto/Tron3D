#include "bikeinputstate.h"

BikeInputState::BikeInputState()
{
	resetState();
}

void BikeInputState::resetState()
{
	m_direction = osg::Vec2d({ 0.0, 0.0 });
}