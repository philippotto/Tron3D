#pragma once

#include <osgGA/GUIEventHandler>

#include "forwarddeclarations.h"

class KeyboardEventHandler : public osgGA::GUIEventHandler
{

public:
	KeyboardEventHandler(BikeInputState* bikeInputState)
	{
		m_bikeInputState = bikeInputState;
	}
	virtual bool handle(const osgGA::GUIEventAdapter& eventAdapter, osgGA::GUIActionAdapter&);
	virtual void accept(osgGA::GUIEventHandlerVisitor& visitor)   { visitor.visit(*this); };

protected:
	BikeInputState* m_bikeInputState;
};
