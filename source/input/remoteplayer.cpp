#include "remoteplayer.h"
// troen
#include "../constants.h"
// other
#include <cmath>
#include "LinearMath/btVector3.h"

using namespace troen::input;

RemotePlayer::RemotePlayer(osg::ref_ptr<BikeInputState> bikeInputState)
{
	m_bikeInputState = bikeInputState;
	m_bikeInputState->setRemote(true);
}

void RemotePlayer::update(troen::networking::bikeUpdateMessage updateMessage)
{
	m_bikeInputState->setPosition(btVector3(updateMessage.x, updateMessage.y, updateMessage.z));
	m_bikeInputState->setAcceleration(updateMessage.acceleration);
	m_bikeInputState->setAngle(updateMessage.turnAngle);
}
