#pragma once

#include <reflectionzeug/Object.h>
#include <scriptzeug/ScriptContext.h>

// OSG
#include <osg/ref_ptr>
// troen
#include "pollingdevice.h"

#include "../util/scriptwatcher.h"

#include "../controller/bikecontroller.h"


using namespace scriptzeug;

namespace troen
{
	namespace input
	{
		class AIScript : public reflectionzeug::Object
		{
		public:
			AIScript(osg::ref_ptr<BikeInputState> bikeInputState, troen::BikeController* bikeController) : reflectionzeug::Object("player") {

				m_bikeController = bikeController;
				m_bikeInputState = bikeInputState;

				addFunction("getFreeDistanceInDirection", this, &AIScript::getFreeDistanceInDirection);
				addFunction("log", this, &AIScript::log);
				addFunction("setTurbo", this, &AIScript::setTurbo);


				addProperty<double>("acceleration", *this, &AIScript::getAcceleration, &AIScript::setAcceleration);
				addProperty<double>("angle", *this, &AIScript::angle, &AIScript::setAngle);
				addProperty<double>("distanceToNextObstacle", *this, &AIScript::distanceToNextObstacle, &AIScript::setDistanceToNextObstacle);
			}


			double getAcceleration() const { return m_acceleration; }
			void setAcceleration(const double &acc) { m_acceleration = acc; }


			double angle() const { return m_angle; }
			void setAngle(const double & angle) { m_angle = angle; }

			double distanceToNextObstacle() const { return m_distanceToNextObstacle; }
			void setDistanceToNextObstacle(const double & distanceToNextObstacle) { m_distanceToNextObstacle = distanceToNextObstacle; }

			double getFreeDistanceInDirection(double angle) {
				double distance = m_bikeController->getDistanceToObstacle(angle);
				return distance;
			}

			void log(std::string message) {
				std::cout << "script log:   " << message << std::endl;
			}

			void setTurbo(bool b) {
				m_bikeInputState->setTurboPressed(b);
			}

		protected:
			double m_angle, m_acceleration, m_distanceToNextObstacle;
			troen::BikeController* m_bikeController;
			osg::ref_ptr<BikeInputState> m_bikeInputState;
		};


		class AI : public PollingDevice
		{
		public:
			//AI(osg::ref_ptr<BikeInputState> bikeInputState);
			AI(osg::ref_ptr<BikeInputState> bikeInputState, troen::BikeController* bikeController);

			void run() override;


		private:
			AIScript *m_aiScript;
			BikeController *m_bikeController;
			
		};

	}
}