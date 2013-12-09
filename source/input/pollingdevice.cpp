#include "pollingdevice.h"
// troen
#include "bikeinputstate.h"

using namespace troen::input;

PollingDevice::PollingDevice(osg::ref_ptr<BikeInputState> bikeInputState)
{
	m_bikeInputState = bikeInputState;
}

bool PollingDevice::refresh()
{
	// subclass responsibility
	return false;
}