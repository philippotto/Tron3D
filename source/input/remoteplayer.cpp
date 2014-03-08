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
	m_newPosition = true;
}

void RemotePlayer::update(troen::networking::bikeUpdateMessage updateMessage)
{
	m_bikeInputState->setPosition(btVector3(updateMessage.x, updateMessage.y, updateMessage.z));
	m_bikeInputState->setRotation(btQuaternion(updateMessage.quat_x, updateMessage.quat_y, updateMessage.quat_z, updateMessage.quat_w));
	m_bikeInputState->setLinearVeloctiy(btVector3(updateMessage.linearVelX, updateMessage.linearVelY, updateMessage.linearVelZ));
	m_bikeInputState->setIsNewPosition(true);
}

