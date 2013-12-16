#include "ai.h"
// troen
#include "bikeinputstate.h"
// other
#include <cmath>

using namespace troen::input;

bool AI::refresh()
{
	m_bikeInputState->setAcceleration(fmod(rand() / 100.0, 1.0));
	m_bikeInputState->setAngle(fmod(rand() / 100.0, 1.0));

	return true;
}