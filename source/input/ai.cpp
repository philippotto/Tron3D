#include <reflectionzeug/Object.h>
#include <scriptzeug/ScriptContext.h>

#include "ai.h"
// troen
#include "bikeinputstate.h"
#include "../constants.h"
#include "../globals.h"
// other
#include <cmath>



using namespace troen::input;


AI::AI(osg::ref_ptr<BikeInputState> bikeInputState) : PollingDevice(bikeInputState)
{


}

void AI::run()
{

	PollingDevice::run();

	m_pollingEnabled = true;
	reflectionzeug::Variant value;

	m_aiScript = new AIScript();

	ScriptContext	*g_scriptingThread = new ScriptContext();

	g_scriptingThread->registerObject(m_aiScript);
	
	while (m_pollingEnabled)
	{
		g_scriptingThread->evaluate("player.acceleration = 1");
		value = g_scriptingThread->evaluate("player.angle = 1");

		m_bikeInputState->setAcceleration(m_aiScript->getAcceleration());
		m_bikeInputState->setAngle(m_aiScript->angle());

		this->msleep(POLLING_DELAY_MS * 2);
	}
}