#include "keyboard.h"
// troen
#include "bikeinputstate.h"
#include "../view/shaders.h"
#include "../constants.h"

using namespace troen::input;

Keyboard::Keyboard(osg::ref_ptr<BikeInputState> bikeInputState, std::vector<osgGA::GUIEventAdapter::KeySymbol> keys)
{
	GUIEventHandler();
	m_bikeInputState = bikeInputState;
	m_keys = keys;
}

bool Keyboard::handle(const osgGA::GUIEventAdapter& eventAdapter, osgGA::GUIActionAdapter& actionAdapter)
{
	switch (eventAdapter.getEventType())
	{
	case(osgGA::GUIEventAdapter::KEYDOWN) :
	{
		int key = eventAdapter.getKey();
		if (key == m_keys[0]) // forward
		{
			m_bikeInputState->setAcceleration(1.0);
			return false;
		}
		else if (key == m_keys[1]) // left
		{
			m_bikeInputState->setAngle(1.0 + m_handbrakePressed * BIKE_HANDBRAKE_FACTOR);
			return false;
		}
		else if (key == m_keys[2]) // backwards
		{
			m_bikeInputState->setAcceleration(-1.0);
			return false;
		}
		else if (key == m_keys[3])
		{
			m_bikeInputState->setAngle(-1.0 - m_handbrakePressed * BIKE_HANDBRAKE_FACTOR);
			return false;
		}
		else if (key == m_keys[4])
		{
			m_handbrakePressed = 1.0;
			return false;
		}
		// todo, move to gameeventhandler
		else if (key == osgGA::GUIEventAdapter::KEY_R)
		{
			std::cout << "Reloading shaders" << std::endl;
			shaders::reloadShaders();
		}
		return false;
	}
	case(osgGA::GUIEventAdapter::KEYUP) :
	{
		int key = eventAdapter.getKey();
		if (key == m_keys[0]) // forward
		{
			m_bikeInputState->setAcceleration(0.0);
			return false;
		}
		else if (key == m_keys[1]) // left
		{
			m_bikeInputState->setAngle(0.0);
			return false;
		}
		else if (key == m_keys[2]) // backwards
		{
			m_bikeInputState->setAcceleration(0.0);
			return false;
		}
		else if (key == m_keys[3])
		{
			m_bikeInputState->setAngle(-0.0);
			return false;
		}
		else if (key == m_keys[4])
		{
			m_handbrakePressed = 0.0;
			return false;
		}
		return false;
	}
	default:
		return false;
	}
}