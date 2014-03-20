#include "remoteplayer.h"
// troen
#include "../constants.h"
// other
#include <cmath>
#include "LinearMath/btVector3.h"
#include "qqueue.h"
#include <memory>

using namespace troen::input;

RemotePlayer::RemotePlayer(osg::ref_ptr<BikeInputState> bikeInputState)
{
	m_bikeInputState = bikeInputState;
	m_bikeInputState->setRemote(true);
	m_newPosition = true;
	m_newFencePositions = std::make_shared<QQueue<btTransform>>();
	m_fenceBufferMutex = std::make_shared<QMutex>();
}

void RemotePlayer::update(troen::networking::bikeUpdateMessage updateMessage)
{
	m_bikeInputState->setPosition(updateMessage.transform.getOrigin());
	m_bikeInputState->setRotation(updateMessage.transform.getRotation());
	m_bikeInputState->setLinearVeloctiy(btVector3(updateMessage.linearVelX, updateMessage.linearVelY, updateMessage.linearVelZ));
	m_bikeInputState->setIsNewPosition(true);
}


void RemotePlayer::addNewFencePosition(btTransform fencePiece)
{
	m_fenceBufferMutex->lock();
	m_newFencePositions->enqueue(fencePiece);
	m_fenceBufferMutex->unlock();
}

bool RemotePlayer::tryGetFencePiece(btTransform &result)
{
	if (!m_newFencePositions->empty())
	{

		m_fenceBufferMutex->lock();
		result = m_newFencePositions->dequeue();
		m_fenceBufferMutex->unlock();
		return true;
	}

	return false;
}

