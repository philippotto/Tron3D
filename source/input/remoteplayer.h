#pragma once
// OSG
#include <osg/ref_ptr>
// troen
#include "pollingdevice.h"
#include "bikeinputstate.h"
#include "../forwarddeclarations.h"
#include "../network/networkmanager.h"


namespace troen
{
	namespace input
	{
		class RemotePlayer
		{
		public:
			RemotePlayer(osg::ref_ptr<BikeInputState> bikeInputState);
			void update(troen::networking::bikeUpdateMessage updateMessage);
		protected:
			osg::ref_ptr<BikeInputState> m_bikeInputState;
		};
	}
}