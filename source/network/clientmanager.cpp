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


void ClientManager::handleSubClassMessages(RakNet::Packet *packet)
{
	switch (packet->data[0])
	{
	case ID_CONNECTION_REQUEST_ACCEPTED:
		//send the ID of the client
		m_connectedToServer = true;
		break;
	case GAME_INIT_PARAMETERS:
		setInitParameters(packet);
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
	default:
		printf("Message with identifier %i has arrived.\n", packet->data[0]);
		break;
	}

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

void ClientManager::setInitParameters(RakNet::Packet *packet)
{
	RakNet::BitStream bsIn(packet->data, packet->length, false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	bsIn.Read(m_gameID);
	bsIn.Read(m_startPosition);
	std::cout << "got game ID: " << m_gameID << std::endl;
	std::cout << "starting at Position" << m_startPosition.getOrigin().x() << " " <<  m_startPosition.getOrigin().y() << std::endl; 
	

}
