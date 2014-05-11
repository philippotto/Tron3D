#pragma once
// OSG
#include <osgGA/GUIEventHandler>
// troen
#include "pollingdevice.h"
#include "../forwarddeclarations.h"

namespace troen
{
namespace input
{
/*! The keyboard class handles user input which is received via keyboard and is intended to navigate the player.*/
	class Keyboard : public osgGA::GUIEventHandler, public PollingDevice
	{

	public:
		Keyboard(osg::ref_ptr<BikeInputState> bikeInputState, std::vector<osgGA::GUIEventAdapter::KeySymbol> keys);
		virtual bool handle(const osgGA::GUIEventAdapter& eventAdapter, osgGA::GUIActionAdapter&);
		virtual void accept(osgGA::GUIEventHandlerVisitor& visitor)   { visitor.visit(*this); };
		void run() override;

	protected:
		osg::ref_ptr<BikeInputState> m_bikeInputState;
		std::vector<osgGA::GUIEventAdapter::KeySymbol> m_keys;
		float m_handbrakePressed = 0;
		bool m_leftPressed = false, m_upPressed = false, m_rightPressed = false, m_downPressed = false, m_turboPressed = false;
		float m_angle = 0;
	};
}
}