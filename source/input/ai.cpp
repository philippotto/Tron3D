#include "ai.h"
// troen
#include "bikeinputstate.h"
#include "../constants.h"
// other
#include <cmath>

using namespace troen::input;

void AI::run()
{
	m_pollingEnabled = true;

	while (m_pollingEnabled)
	{
		m_bikeInputState->setAcceleration(fmod(rand() / 100.0, 1.0));
		m_bikeInputState->setAngle(fmod(rand() / 100.0, 1.0));

		this->msleep(POLLING_DELAY_MS * 2);
	}
}