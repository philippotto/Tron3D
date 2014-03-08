#include "networkmanager.h"
//std
#include <stdio.h>
#include <string.h>
#include <initializer_list>

//qt
#include <QQueue>
//bullet
#include "LinearMath/btQuaternion.h"
//troen
#include "../constants.h"
#include "../troengame.h"
#include "../controller/bikecontroller.h"
#include "../model/bikemodel.h"
#include "../player.h"

//raknet


using namespace troen::networking;
#define MAX_CLIENTS 10
#define SERVER_PORT 60000






NetworkManager::NetworkManager(troen::TroenGame *game)
{
	m_packet = new RakNet::Packet;
	peer = RakNet::RakPeerInterface::GetInstance();
	m_sendUpdateMessagesQueue = new QQueue<bikeUpdateMessage>();
	m_sendInputUpdateMessagesQueue = new QQueue<bikeInputUpdateMessage>();
	m_sendStatusUpdateMessage = new QQueue<bikeStatusMessage>();
	m_remotePlayers = std::vector<std::shared_ptr<input::RemotePlayer>>();
	m_sendBufferMutex = new QMutex();
	
	m_localBikeController = NULL;
	m_lastUpdateTime = 0;
	m_gameID = 0;
	m_troenGame = game;
	m_gameStarted = false;
}

void  NetworkManager::enqueueMessage(bikeUpdateMessage message)
{
	m_sendBufferMutex->lock();
	m_sendUpdateMessagesQueue->enqueue(message);
	m_sendBufferMutex->unlock();
}

void  NetworkManager::enqueueMessage(bikeInputUpdateMessage message)
{
	m_sendBufferMutex->lock();
	m_sendInputUpdateMessagesQueue->enqueue(message);
	m_sendBufferMutex->unlock();
}

void  NetworkManager::enqueueMessage(bikeStatusMessage message)
{
	m_sendBufferMutex->lock();
	m_sendStatusUpdateMessage->enqueue(message);
	m_sendBufferMutex->unlock();
}





void NetworkManager::registerRemotePlayer(std::shared_ptr<troen::input::RemotePlayer> remotePlayer)
{
	m_remotePlayers.push_back(remotePlayer);
}

void NetworkManager::registerLocalBikeController(std::shared_ptr<troen::BikeController> controller)
{
	m_localBikeController = controller;
}


void NetworkManager::sendPoints(int pointCount, int status, short secondBike)
{
	bikeStatusMessage message = { m_gameID, status, pointCount, secondBike };
	enqueueMessage(message);
}

void NetworkManager::receiveStatusMessage(bikeStatusMessage message)
{
	if (is_in(message.status, { (int)PLAYER_DEATH_ON_WALL, (int)PLAYER_DEATH_ON_OWN_FENCE, (int)PLAYER_DEATH_ON_OTHER_PLAYER }))
	{

		for (auto player : m_troenGame->players())
		{
			if (player->isRemote())
			{
				player->increaseDeathCount();
			}
		}
	}
}


void NetworkManager::update(long double g_gameTime)
{
	if (this->isValidSession())
	{
		btVector3 pos = m_localBikeController->getModel()->getPositionBt();
		btQuaternion quat = m_localBikeController->getModel()->getRotationQuat();
		btVector3 linearVelocity = m_localBikeController->getModel()->getLinearVelocity();
		btVector3 angularVelocity = m_localBikeController->getModel()->getAngularVelocity();
		bikeUpdateMessage message = {
			m_gameID,
			pos.x(), pos.y(), pos.z(),
			quat.x(), quat.y(), quat.z(), quat.w(),
			linearVelocity.x(), linearVelocity.y(), linearVelocity.z()
		};

		if ((message.linearVelX != lastSentMessage.linearVelX) || (message.linearVelY != lastSentMessage.linearVelY) || (message.linearVelZ != lastSentMessage.linearVelZ) ||
			(message.quat_x != lastSentMessage.quat_x) || (message.quat_y != lastSentMessage.quat_y) || (message.quat_z != lastSentMessage.quat_z) || (message.quat_w != lastSentMessage.quat_w) ||
			g_gameTime - m_lastUpdateTime > 20.0)
		{
			enqueueMessage(message);
			lastSentMessage = message;
		}

	}
}



//!! This runs in a seperate thread //
void NetworkManager::run()
{

	// subclass responsibility
}


void NetworkManager::sendData()
{
	if (isValidSession())
	{
		while (!m_sendUpdateMessagesQueue->empty())
		{

			RakNet::BitStream bsOut;
			// Use a BitStream to write a custom user message
			//Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically
			bsOut.Write((RakNet::MessageID)BIKE_POSITION_MESSSAGE);
			m_sendBufferMutex->lock();
			messageToSend = m_sendUpdateMessagesQueue->dequeue();
			m_sendBufferMutex->unlock();
			bsOut.Write(messageToSend);
			//bsOut.SerializeFloat16(true,)


			//peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
			peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);


		}

		while (!m_sendStatusUpdateMessage->empty())
		{
			RakNet::BitStream bsOut;
			// Use a BitStream to write a custom user message
			//Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically
			bsOut.Write((RakNet::MessageID)BIKE_STATUS_MESSAGE);
			m_sendBufferMutex->lock();
			statusMessageToSend = m_sendStatusUpdateMessage->dequeue();
			m_sendBufferMutex->unlock();
			bsOut.Write(statusMessageToSend);

			//peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
			peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		}
	}
}

void NetworkManager::synchronizeGameStart()
{
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)GAME_START_MESSAGE);
	peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	m_gameStarted = true;
}



bool NetworkManager::isValidSession()
{
	//sublcass responsibilty
	return true;
}

