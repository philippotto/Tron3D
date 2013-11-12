#pragma once
// OSG
#include <osgGA/GUIEventHandler>
// troen
#include "../forwarddeclarations.h"

namespace troen
{
namespace input
{
	class Keyboard : public osgGA::GUIEventHandler
	{

	public:
		Keyboard(osg::ref_ptr<BikeInputState> bikeInputState);
		virtual bool handle(const osgGA::GUIEventAdapter& eventAdapter, osgGA::GUIActionAdapter&);
		virtual void accept(osgGA::GUIEventHandlerVisitor& visitor)   { visitor.visit(*this); };

	protected:
		osg::ref_ptr<BikeInputState> m_bikeInputState;
	};
}
}