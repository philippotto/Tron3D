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
#include "qcolor.h"

//raknet


using namespace troen::networking;
#define MAX_CLIENTS 10
#define SERVER_PORT 60000



ServerManager::ServerManager(troen::TroenGame *game) : NetworkManager(game)
{
	m_startPositions = std::make_shared<std::vector<btTransform>>();
	btVector3 Z_AXIS(0, 0, 1);
	m_startPositions->push_back(btTransform(btQuaternion(Z_AXIS, (float)PI * 3.f / 4.f), btVector3(20, 20, BIKE_DIMENSIONS.z() / 2 + 500)));
	m_startPositions->push_back(btTransform(btQuaternion(Z_AXIS, (float)PI * 3.f / 4.f), btVector3(40, 20, BIKE_DIMENSIONS.z() / 2 + 500)));
	m_startPositions->push_back(btTransform(btQuaternion(Z_AXIS, (float)-PI * 1.f / 4.f), btVector3(60, 20, BIKE_DIMENSIONS.z() / 2 + 500)));
	m_startPositions->push_back(btTransform(btQuaternion(Z_AXIS, (float)-PI * 3.f / 4.f), btVector3(80, 20, BIKE_DIMENSIONS.z() / 2 + 500)));
	m_startPositions->push_back(btTransform(btQuaternion(Z_AXIS, 0), btVector3(100, 100, BIKE_DIMENSIONS.z() / 2 + 500)));
	m_startPositions->push_back(btTransform(btQuaternion(Z_AXIS, 0), btVector3(-100, -100, BIKE_DIMENSIONS.z() / 2 + 500)));
	
	m_startPosition = m_startPositions->at(0);

	m_gameID = 0;

	m_numClientsConnected = 0;
}


void ServerManager::handleSubClassMessages(RakNet::Packet *packet)
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
			giveIDtoClient(packet);
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
		case REGISTER_PLAYER_AT_SERVER:
			registerClient(packet);
			break;
		default:
			printf("Message with identifier %i has arrived.\n", packet->data[0]);
			break;
	}
}


void ServerManager::openServer()
{
	m_isServer = true;
	RakNet::SocketDescriptor sd(SERVER_PORT, 0);
	peer->Startup(MAX_CLIENTS, &sd, 1);

	printf("Starting the server.\n");
	// We need to let the server accept incoming connections from the clients
	peer->SetMaximumIncomingConnections(MAX_CLIENTS);

	//alice -> for debugging
	m_ownPlayerInfo = std::make_shared<NetworkPlayerInfo>("alice", getPlayerColor(m_gameID), m_gameID, false, m_startPosition);
	m_players.push_back(m_ownPlayerInfo);



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


void ServerManager::giveIDtoClient(RakNet::Packet *packet)
{
	m_numClientsConnected++;

	RakNet::BitStream bsOut;
	//set remote GameID
	bsOut.Write((RakNet::MessageID)GAME_INIT_PARAMETERS);
	bsOut.Write(m_numClientsConnected);
	//send a btTransform startposition
	bsOut.Write(m_startPositions->at(m_numClientsConnected));
	peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, packet->systemAddress, false);
	printf("A connection is incoming.\n");

}



void ServerManager::registerClient(RakNet::Packet *packet)
{
	addPlayer(packet);

	//send info of all existing players to client
	for (auto player : m_players)
	{
		RakNet::BitStream bsAddPlayer;
		bsAddPlayer.Write((RakNet::MessageID)ADD_PLAYER);

		player->serialize(&bsAddPlayer);

		peer->Send(&bsAddPlayer, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, packet->systemAddress, false);
	}


}