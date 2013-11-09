#pragma once

#include <osgGA/GUIEventHandler>

#include "../forwarddeclarations.h"

class KeyboardEventHandler : public osgGA::GUIEventHandler
{

public:
	KeyboardEventHandler(BikeInputState* bikeInputState);
	virtual bool handle(const osgGA::GUIEventAdapter& eventAdapter, osgGA::GUIActionAdapter&);
	virtual void accept(osgGA::GUIEventHandlerVisitor& visitor)   { visitor.visit(*this); };

protected:
	osg::ref_ptr<BikeInputState> m_bikeInputState;

private:
	bool m_keyPressed;
};
