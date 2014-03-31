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
		/*! The PollingDevice is an abstract class which inherits from QThread. It is used as the base class for controller input and AI.*/
		class PollingDevice : public QThread
		{
		public:
			PollingDevice(osg::ref_ptr<BikeInputState> bikeInputState);
			virtual void run();
			void stop();
			virtual void setVibration(const bool b) { m_vibrationEnabled = b; };
		protected:
			osg::ref_ptr<BikeInputState> m_bikeInputState;

			bool m_pollingEnabled = false;
			bool m_vibrationEnabled = false;
		};
	}
}