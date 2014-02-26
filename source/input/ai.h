#pragma once

#include <reflectionzeug/Object.h>
#include <scriptzeug/ScriptContext.h>

// OSG
#include <osg/ref_ptr>
// troen
#include "pollingdevice.h"

using namespace scriptzeug;

namespace troen
{
	namespace input
	{
		class AIScript : public reflectionzeug::Object
		{
		public:
			AIScript() : reflectionzeug::Object("player") {
				addProperty<double>("acceleration", *this, &AIScript::getAcceleration, &AIScript::setAcceleration);
				addProperty<double>("angle", *this, &AIScript::angle, &AIScript::setAngle);
			}


			double getAcceleration() const { return m_acceleration; }
			void setAcceleration(const double &acc) { m_acceleration = acc; }
			
			
			double angle() const { return m_angle; }
			void setAngle(const double & angle) { m_angle = angle; }

		protected:
			double m_angle, m_acceleration ;
		};

		class AI : public PollingDevice
		{
		public:
			AI(osg::ref_ptr<BikeInputState> bikeInputState);
			
			void run() override;
			

		private:
			AIScript *m_aiScript;
		};

	}
}