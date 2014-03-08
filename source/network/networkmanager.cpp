#include "networkmanager.h"
//std
#include <stdio.h>
#include <string.h>
#include <initializer_list>
#include "MessageIdentifiers.h"
//qt
#include <QQueue>
//bullet
#include "LinearMath/btQuaternion.h"
//troen
#include "../constants.h"
#include "../troengame.h"
#include "../controller/bikecontroller.h"
#include "../model/bikemodel.h"

//raknet


using namespace troen::networking;
#define MAX_CLIENTS 10
#define SERVER_PORT 60000


enum GameMessages
{
	BIKE_POSITION_MESSSAGE = ID_USER_PACKET_ENUM + 1,
	GAME_START_MESSAGE = ID_USER_PACKET_ENUM + 2,
	BIKE_STATUS_MESSAGE = ID_USER_PACKET_ENUM + 3,
	GAME_SET_ID = ID_USER_PACKET_ENUM + 4
};

struct bikeUpdateMessage receivedUpdateMessage, lastSentMessage, messageToSend;
struct bikeStatusMessage receivedStatusMessage, statusMessageToSend;

NetworkManager::NetworkManager(troen::TroenGame *game)
{
	m_packet = new RakNet::Packet;
	peer = RakNet::RakPeerInterface::GetInstance();
	m_connectedToServer = false;
	m_numClientsConnected = 0;
	m_sendUpdateMessagesQueue = new QQueue<bikeUpdateMessage>();
	m_sendInputUpdateMessagesQueue = new QQueue<bikeInputUpdateMessage>();
	m_sendStatusUpdateMessage = new QQueue<bikeStatusMessage>();
	m_sendBufferMutex = new QMutex();
	m_localBikeController = NULL;
	m_lastUpdateTime = 0;
	m_gameID = 0;
	m_troenGame = game;
	m_remotePlayers = std::vector<std::shared_ptr<input::RemotePlayer>>();
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
	if (is_in(message.status, { (int)PLAYER_DEATH_ON_WALL, (int)PLAYER_DEATH_ON_OWN_FENCE, (int) PLAYER_DEATH_ON_OTHER_PLAYER }))
	{

		for (auto controller : m_troenGame->getBikeControllers())
		{
			if (controller->isRemote())
			{
				controller->increaseDeathCount();
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
			linearVelocity.x(), linearVelocity.y(), linearVelocity.z(), 
			angularVelocity.z()
		};

		//bikeInputUpdateMessage inputmessage = { m_localBikeController->getInputAngle(), m_localBikeController->getInputAcceleration() };
		if ((message.linearVelX != lastSentMessage.linearVelX) || (message.linearVelY != lastSentMessage.linearVelY) ||
			(message.angularVelZ != lastSentMessage.angularVelZ) || g_gameTime - m_lastUpdateTime > 20.0)
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
	RakNet::Packet *packet;
	while (1)
	{
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			switch (packet->data[0])
			{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Another client has disconnected.\n");
				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("Another client has lost the connection.\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("Another client has connected.\n");
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				//send the ID of the client
				m_connectedToServer = true;
				break;
			case GAME_SET_ID:
			{
								RakNet::BitStream bsIn(packet->data, packet->length, false);
								bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
								short ID;
								bsIn.Read(ID);
								m_gameID = ID;
								printf("Got game ID.\n");
			}
				break;

			case ID_NEW_INCOMING_CONNECTION:
			{
											   m_numClientsConnected++;

											   RakNet::BitStream bsOut;
											   bsOut.Write((RakNet::MessageID)GAME_SET_ID);
											   bsOut.Write(m_numClientsConnected);
											   peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, packet->systemAddress, false);
											   m_numClientsConnected = true;
											   printf("A connection is incoming.\n");
			}
				break;

			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				if (m_isServer){
					printf("A client has disconnected.\n");
				}
				else {
					printf("We have been disconnected.\n");
				}
				break;
			case ID_CONNECTION_LOST:
				if (m_isServer){
					printf("A client lost the connection.\n");
				}
				else {
					printf("Connection lost.\n");
				}
				break;

			case BIKE_POSITION_MESSSAGE:
			{
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(receivedUpdateMessage);
				std::cout << receivedUpdateMessage.x << " " << receivedUpdateMessage.y << " " << receivedUpdateMessage.z << " " << receivedUpdateMessage.linearVelX << " " << receivedUpdateMessage.linearVelY << std::endl;
				m_remotePlayers[0]->update(receivedUpdateMessage);
			}
				break;

			case BIKE_STATUS_MESSAGE:
			{
										RakNet::BitStream bsIn(packet->data, packet->length, false);
										bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
										bsIn.Read(receivedStatusMessage);
										//receiveStatusMessage(receivedStatusMessage);
										printf("status_message");

			}
				break;

			case GAME_START_MESSAGE:
			{
									   //prevent game from calling start two times due to receviment of own packet
									   if (!m_gameStarted)
									   {

										   emit remoteStartCall();
										   m_gameStarted = true;
									   }
			}
				break;

			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}
		}


		sendData();
		this->msleep(10);
		
	}

	//cleanup
	RakNet::RakPeerInterface::DestroyInstance(peer);
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

void NetworkManager::openServer()
{
	m_isServer = true;
	RakNet::SocketDescriptor sd(SERVER_PORT, 0);
	peer->Startup(MAX_CLIENTS, &sd, 1);

	printf("Starting the server.\n");
	// We need to let the server accept incoming connections from the clients
	peer->SetMaximumIncomingConnections(MAX_CLIENTS);
	

	//calls the run method in a seperate thread
	start();
}
void NetworkManager::openClient(std::string connectAddr)
{
	m_isServer = false;
	RakNet::SocketDescriptor sd;
	peer->Startup(1, &sd, 1);

	printf("Starting the client.\n");
	RakNet::ConnectionAttemptResult r = peer->Connect(connectAddr.c_str(), SERVER_PORT, 0, 0);

	//calls the run method in a seperate thread
	start();
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
	//for now
	return (m_connectedToServer && (m_gameID > 0)) || ( m_numClientsConnected > 0);

}


std::string  NetworkManager::getClientAddress() 
{ 
	return m_clientAddress;
}
