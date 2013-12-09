#pragma once
// OSG
#include <osg/ref_ptr>
// troen
#include "../forwarddeclarations.h"

namespace troen
{
	namespace input
	{
		class PollingDevice
		{
		public:
			PollingDevice(osg::ref_ptr<BikeInputState> bikeInputState);
			virtual bool refresh();

		protected:
			osg::ref_ptr<BikeInputState> m_bikeInputState;
		};
	}
}