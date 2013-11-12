#include "keyboard.h"
// STD
#include <iostream>
// troen
#include "bikeinputstate.h"

using namespace troen::input;

Keyboard::Keyboard(osg::ref_ptr<BikeInputState> bikeInputState)
{
	m_bikeInputState = bikeInputState;
}

bool Keyboard::handle(const osgGA::GUIEventAdapter& eventAdapter, osgGA::GUIActionAdapter& actionAdapter)
{
	switch (eventAdapter.getEventType())
	{
	case(osgGA::GUIEventAdapter::KEYDOWN) :
	{
		switch (eventAdapter.getKey())
		{
		case 'w':
			std::cout << "[Keyboard::handle] w key pressed" << std::endl;
			m_bikeInputState->setAcceleration(1.0);
			return false;
			break;
		case 'a':
			std::cout << "[Keyboard::handle] a key pressed" << std::endl;
			m_bikeInputState->setAngle(1.0);
			return false;
			break;
		case 's':
			std::cout << "[Keyboard::handle] s key pressed" << std::endl;
			m_bikeInputState->setAcceleration(-1.0);
			return false;
			break;
		case 'd':
			std::cout << "[Keyboard::handle] d key pressed" << std::endl;
			m_bikeInputState->setAngle(-1.0);
			return false;
			break;
		default:
			return false;
		}
	}
	case(osgGA::GUIEventAdapter::KEYUP) :
	{
		switch (eventAdapter.getKey())
		{
		case 'w':
			std::cout << "[Keyboard::handle] w key released" << std::endl;
			m_bikeInputState->setAcceleration(0.0);
			return false;
			break;
		case 'a':
			std::cout << "[Keyboard::handle] a key released" << std::endl;
			m_bikeInputState->setAngle(0.0);
			return false;
			break;
		case 's':
			std::cout << "[Keyboard::handle] s key released" << std::endl;
			m_bikeInputState->setAcceleration(0.0);
			return false;
			break;
		case 'd':
			std::cout << "[Keyboard::handle] d key released" << std::endl;
			m_bikeInputState->setAngle(0.0);
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