#pragma once
// OSG
#include <osg/ref_ptr>
// troen
#include "pollingdevice.h"

#include <scriptzeug/ScriptEnvironment.h>
#include <scriptzeug/Scriptable.h>

using namespace scriptzeug;

namespace troen
{
	namespace input
	{
		class AIScript : public Scriptable
		{
		public:
			AIScript() {
				//addProperty<float>("acceleration", *this, &AI::acceleration, &AI::setAcceleration);
				//addProperty<float>("angle", *this, &AI::angle, &AI::setAngle);

				//addFunction("setAcceleration", this, &AIScript::setAcceleration);
				//addFunction("getAcceleration", this, &AI::getAcceleration);
			}


			float m_acceleration = 0;
			float acceleration() const
			{
				return m_acceleration;
			}

			void setAcceleration(float acc)
			{
				m_acceleration = acc;
			}


			float m_angle = 0;
			float angle() const {
				return m_angle;
			}

			void setAngle(const float & angle) {
				m_angle = angle;
			}
		};

		class AI : public PollingDevice
		{
		public:
			AI(osg::ref_ptr<BikeInputState> bikeInputState);
			void run() override;

		private:
			AIScript m_aiScript;
		};

	}
}