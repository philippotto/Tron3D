#include "remoteplayer.h"
// troen
#include "../constants.h"
// other
#include <cmath>

using namespace troen::input;

RemotePlayer::RemotePlayer(osg::ref_ptr<BikeInputState> bikeInputState)
{
	m_bikeInputState = bikeInputState;
}

void RemotePlayer::update(troen::networking::bikeUpdateMessage updateMessage)
{
	m_bikeInputState->setAcceleration(updateMessage.acceleration);
	m_bikeInputState->setAngle(updateMessage.turnAngle);
}
