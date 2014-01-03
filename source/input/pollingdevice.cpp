#include "pollingdevice.h"
// troen
#include "bikeinputstate.h"

using namespace troen::input;

PollingDevice::PollingDevice(osg::ref_ptr<BikeInputState> bikeInputState)
{
	m_bikeInputState = bikeInputState;
}

void PollingDevice::run()
{
	// subclass responsibility
}

void PollingDevice::stop(){
	m_pollingEnabled = false;
}