#include "keyboardeventhandler.h"

#include <iostream>

#include "bikeinputstate.h"

using namespace troen::input;

#define MOVE_VALUE 0.5
#define ROTATION_VALUE 3.0

KeyboardEventHandler::KeyboardEventHandler(BikeInputState* bikeInputState)
{
	m_bikeInputState = bikeInputState;
}

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
			m_bikeInputState->m_acceleration = MOVE_VALUE;
			return false;
			break;
		case 'a':
			std::cout << " a key pressed" << std::endl;
			m_bikeInputState->m_angle = ROTATION_VALUE;
			return false;
			break;
		case 's':
			std::cout << " s key pressed" << std::endl;
			m_bikeInputState->m_acceleration = -MOVE_VALUE;
			return false;
			break;
		case 'd':
			std::cout << " d key pressed" << std::endl;
			m_bikeInputState->m_angle = -ROTATION_VALUE;
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
			std::cout << " w key released" << std::endl;
			m_bikeInputState->m_acceleration = 0.0;
			return false;
			break;
		case 'a':
			std::cout << " a key released" << std::endl;
			m_bikeInputState->m_angle = 0.0;
			return false;
			break;
		case 's':
			std::cout << " s key released" << std::endl;
			m_bikeInputState->m_acceleration = 0.0;
			return false;
			break;
		case 'd':
			std::cout << " d key released" << std::endl;
			m_bikeInputState->m_angle = 0.0;
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