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



AI::AI(osg::ref_ptr<BikeInputState> bikeInputState, troen::BikeController* bikeController) : PollingDevice(bikeInputState)
{
	m_bikeController = bikeController;

}

void AI::run()
{
	PollingDevice::run();

	m_pollingEnabled = true;
	
	m_aiScript = new AIScript(m_bikeInputState, m_bikeController);
	ScriptContext	*g_scriptingThread = new ScriptContext();
	g_scriptingThread->registerObject(m_aiScript);

	ScriptWatcher scriptWatcher;
	scriptWatcher.watchAndLoad("source/scripts/ai.js", g_scriptingThread);
	
	while (m_pollingEnabled)
	{
		m_aiScript->setDistanceToNextObstacle(m_bikeController->getDistanceToObstacle(0));

		reflectionzeug::Variant value;
		value = g_scriptingThread->evaluate("try {     if (typeof move !== 'undefined') { move() } else { player.angle = 0 }      } catch (ex) { ex } ");
		//std::cout << "result of script: " << value.toString() << std::endl;

		m_bikeInputState->setAcceleration(m_aiScript->getAcceleration());
		m_bikeInputState->setAngle(m_aiScript->angle());

		QCoreApplication::processEvents();
		this->msleep(POLLING_DELAY_MS * 2);
	}
}