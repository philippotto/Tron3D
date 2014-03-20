#include <reflectionzeug/Object.h>
#include <scriptzeug/ScriptContext.h>

#include "ai.h"
// troen
#include "bikeinputstate.h"
#include "../constants.h"
#include "../globals.h"
// other
#include <cmath>


#include "../controller/bikecontroller.h"

using namespace troen::input;

AI::AI(osg::ref_ptr<BikeInputState> bikeInputState, troen::BikeController* bikeController)
	: PollingDevice(bikeInputState),
	  reflectionzeug::Object("player")
{
	m_bikeController = bikeController;
	m_bikeInputState = bikeInputState;

	addFunction("getFreeDistanceInDirection", this, &AI::getFreeDistanceInDirection);
	addFunction("getID", this, &AI::getID);
	addFunction("getHealth", this, &AI::getHealth);
	addFunction("log", this, &AI::log);
	addFunction("setTurbo", this, &AI::setTurbo);

	addProperty<double>("acceleration", *this, &AI::getAcceleration, &AI::setAcceleration);
	addProperty<double>("angle", *this, &AI::angle, &AI::setAngle);
	addProperty<double>("distanceToNextObstacle", *this, &AI::distanceToNextObstacle, &AI::setDistanceToNextObstacle);
}

void AI::run()
{
	PollingDevice::run();

	m_pollingEnabled = true;
	
	ScriptContext g_scriptingThread;
	g_scriptingThread.registerObject(this);

	ScriptWatcher scriptWatcher;
	scriptWatcher.watchAndLoad("scripts/ai.js", &g_scriptingThread);
	
	while (m_pollingEnabled)
	{
		setDistanceToNextObstacle(m_bikeController->getDistanceToObstacle(0));

		reflectionzeug::Variant value;
		value = g_scriptingThread.evaluate("try {     if (typeof move !== 'undefined') { move() } else { player.angle = 0 }      } catch (ex) { ex } ");
		
		m_bikeInputState->setAcceleration(getAcceleration());
		m_bikeInputState->setAngle(angle());

		QCoreApplication::processEvents();
		this->msleep(POLLING_DELAY_MS * 2);
	}
}