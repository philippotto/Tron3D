#include "ai.h"
// troen
#include "bikeinputstate.h"
#include "../constants.h"
#include "../globals.h"
// other
#include <cmath>

#include <scriptzeug/ScriptEnvironment.h>
#include <scriptzeug/Scriptable.h>


using namespace troen::input;


AI::AI(osg::ref_ptr<BikeInputState> bikeInputState) : PollingDevice(bikeInputState)
{


}


void AI::run()
{
	m_pollingEnabled = true;

	//std::cout << "m_aiscript" << m_aiScript;
	std::cout << "m_aiscript" << &m_aiScript << std::endl;

	std::cout << "m_aiscript" << m_aiScript.acceleration() << std::endl;
	


	g_scripting->registerObject("player11", &m_aiScript);

	while (m_pollingEnabled)
	{
		//g_scripting->evaluate("player.setAcceleration(1)");
		//g_scripting->evaluate("player");
		//g_scripting->evaluate("1+1");

		m_bikeInputState->setAcceleration(m_aiScript.acceleration());
		m_bikeInputState->setAngle(m_aiScript.angle());

		this->msleep(POLLING_DELAY_MS * 2);
	}
}