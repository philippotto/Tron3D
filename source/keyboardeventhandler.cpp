#include "keyboardeventhandler.h"

#include <iostream>

#include <osg/Vec2>

#include "bikeinputstate.h"

bool KeyboardEventHandler::handle(const osgGA::GUIEventAdapter& eventAdapter, osgGA::GUIActionAdapter& actionAdapter)
{
	switch (eventAdapter.getEventType())
	{
	case(osgGA::GUIEventAdapter::KEYDOWN) :
	{
		switch (eventAdapter.getKey())
		{
		case 'w':
			std::cout << " w key pressed" << std::endl;
			m_bikeInputState->direction = osg::Vec2d({ 0.0, 1.0 });
			return false;
			break;
		case 'a':
			std::cout << " a key pressed" << std::endl;
			m_bikeInputState->direction = osg::Vec2d({ -1.0, 0.0 });
			return false;
			break;
		case 's':
			std::cout << " s key pressed" << std::endl;
			m_bikeInputState->direction = osg::Vec2d({ 0.0, -1.0 });
			return false;
			break;
		case 'd':
			std::cout << " d key pressed" << std::endl;
			m_bikeInputState->direction = osg::Vec2d({ 1.0, 0.0 });
			return false;
			break;
		default:
			return false;
		}
	}
	default:
		return false;
	}
}