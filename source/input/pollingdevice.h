#pragma once
// OSG
#include <osg/ref_ptr>
// Qt
#include <QThread>
// troen
#include "../forwarddeclarations.h"

namespace troen
{
	namespace input
	{
		class PollingDevice : public QThread
		{
		public:
			PollingDevice(osg::ref_ptr<BikeInputState> bikeInputState);
			virtual void run();
			void stop();

		protected:
			osg::ref_ptr<BikeInputState> m_bikeInputState;

			bool m_pollingEnabled = false;
		};
	}
}