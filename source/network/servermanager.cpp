#include "servermanager.h"
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



ServerManager::ServerManager(troen::TroenGame *game) : NetworkManager(game)
{

	m_numClientsConnected = 0;
}



//!! This runs in a seperate thread //
void ServerManager::run()
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

			case ID_NEW_INCOMING_CONNECTION:
			{
				m_numClientsConnected++;

				RakNet::BitStream bsOut;
				//set remote GameID
				bsOut.Write((RakNet::MessageID)GAME_SET_ID);
				bsOut.Write(m_numClientsConnected);
				peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, packet->systemAddress, false);
				printf("A connection is incoming.\n");
			}
				break;

			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				printf("A client has disconnected.\n");
				break;
			case ID_CONNECTION_LOST:
				printf("A client lost the connection.\n");
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


void ServerManager::openServer()
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


bool ServerManager::isValidSession()
{
	//for now
	return (m_numClientsConnected > 0);

}


std::string  ServerManager::getClientAddress()
{
	return m_clientAddress;
}
