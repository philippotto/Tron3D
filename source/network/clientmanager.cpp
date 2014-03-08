#include "clientmanager.h"
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
#include "../player.h"

//raknet


using namespace troen::networking;
#define MAX_CLIENTS 10
#define SERVER_PORT 60000


ClientManager::ClientManager(troen::TroenGame *game) : NetworkManager(game)
{
	m_connectedToServer = false;
}


//!! This runs in a seperate thread //
void ClientManager::run()
{

	// subclass responsibility
	RakNet::Packet *packet;
	while (1)
	{
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			switch (packet->data[0])
			{
			case ID_CONNECTION_REQUEST_ACCEPTED:
				//send the ID of the client
				m_connectedToServer = true;
				break;
			case GAME_SET_ID:
			{
								RakNet::BitStream bsIn(packet->data, packet->length, false);
								bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
								bsIn.Read(m_gameID);
								std::cout << "got game ID: " << m_gameID << std::endl;
			}
				break;

			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
					printf("We have been disconnected.\n");
				break;
			case ID_CONNECTION_LOST:
				printf("Connection lost.\n");
				break;

			case BIKE_POSITION_MESSSAGE:
			{
										   readMessage(packet, receivedUpdateMessage);
										   m_remotePlayers[0]->update(receivedUpdateMessage);
			}
				break;

			case BIKE_STATUS_MESSAGE:
			{
										readMessage(packet, receivedStatusMessage);
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


void ClientManager::openClient(std::string connectAddr)
{
	m_isServer = false;
	RakNet::SocketDescriptor sd;
	peer->Startup(1, &sd, 1);

	printf("Starting the client.\n");
	RakNet::ConnectionAttemptResult r = peer->Connect(connectAddr.c_str(), SERVER_PORT, 0, 0);

	//calls the run method in a seperate thread
	start();
}


bool ClientManager::isValidSession()
{
	//for now
	return (m_connectedToServer && (m_gameID > 0));

}

