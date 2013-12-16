#pragma once
// OSG
#include <osg/ref_ptr>
// troen
#include "pollingdevice.h"

namespace troen
{
	namespace input
	{
		class AI : public PollingDevice
		{
		public:
			AI(osg::ref_ptr<BikeInputState> bikeInputState) : PollingDevice(bikeInputState) {};
			bool refresh() override;
		};
	}
}