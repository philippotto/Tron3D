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



ServerManager::ServerManager(troen::TroenGame *game, std::vector<QString> playerNames) : NetworkManager(game)
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


	m_numClientsConnected = 0;
	m_playerNames = playerNames;

	//gameid also is a count of the server local player + AIs
	//clients will have increasing ids from this
	m_gameID = m_playerNames.size()-1;
	for (int i = 0; i < playerNames.size(); i++)
	{
		std::shared_ptr<NetworkPlayerInfo> ownPlayer = std::make_shared<NetworkPlayerInfo>(m_playerNames[i], getPlayerColor(i), i, false, m_startPositions->at(i));
		m_ownPlayersInfo.push_back(ownPlayer);
		m_players.push_back(ownPlayer);
	}

	m_initialReset = false;
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
	peer->SetOccasionalPing(true);





	//calls the run method in a seperate thread
	start();
}


bool ServerManager::isValidSession()
{
	//for now
	return (m_numClientsConnected > 0);

}

void ServerManager::update(long double g_gameTime)
{
	NetworkManager::update(g_gameTime);

	//after 1 second, reset all players score to 0, this is a workaround for some obscure synchronization issues at start
	if (!m_initialReset && g_gameTime > 1000)
	{
		m_initialReset = true;
		for (auto player : m_troenGame->players())
		{
			NetworkPlayerInfo *netplayer = getPlayerWithID(player->getNetworkID()).get();
			player->setKillCount(0);

			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID)GAME_STATUS_MESSAGE);
			gameStatusMessage message = { player->getNetworkID(), RESET_SCORE, NULL, NULL };
			bsOut.Write(message);
			//relay message to all systems
			peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);

		}
	}
}



void ServerManager::giveIDtoClient(RakNet::Packet *packet)
{
	m_numClientsConnected++;

	RakNet::BitStream bsOut;
	//set remote GameID
	bsOut.Write((RakNet::MessageID)GAME_INIT_PARAMETERS);
	bsOut.Write(m_gameID + m_numClientsConnected);
	//send a btTransform startposition
	bsOut.Write(m_startPositions->at(m_gameID + m_numClientsConnected));
	peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, packet->systemAddress, false);
	printf("A connection is incoming.\n");

}



bool ServerManager::addPlayer(RakNet::Packet *packet)
{

	if (!NetworkManager::addPlayer(packet))
		return false;

	//send player info to all other clients
	RakNet::BitStream bsAddPlayer;
	bsAddPlayer.Write((RakNet::MessageID)ADD_PLAYER);
	//write the player infos into the bitstream
	m_players.back()->serialize(&bsAddPlayer);
	//broadcast to all connected clients except sending client
	peer->Send(&bsAddPlayer, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, packet->systemAddress, true);


	//send info of all existing players to client
	for (auto player : m_players)
	{
		RakNet::BitStream bsAddPlayer;
		bsAddPlayer.Write((RakNet::MessageID)ADD_PLAYER);

		player->serialize(&bsAddPlayer);

		peer->Send(&bsAddPlayer, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, packet->systemAddress, false);
	}

	return true;

}


void ServerManager::setLocalGameReady()
{
	for (int i = 0; i <= m_gameID; i++)
	{

		getPlayerWithID(i)->status = WAITING_FOR_GAMESTART;

		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)BIKE_STATUS_MESSAGE);
		getPlayerWithID(i)->serializeStatus(&bsOut);
		peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	}
}


void ServerManager::handleBikePositionMessage(bikeUpdateMessage message, RakNet::SystemAddress address)
{
	NetworkManager::handleBikePositionMessage(message, address);

	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)BIKE_POSITION_MESSSAGE);
	bsOut.Write(message);
	//relay message to all systems except the origin of the message
	peer->Send(&bsOut, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0, address, true);

}

void ServerManager::handleBikeStatusMessage(bikeStatusMessage message, RakNet::SystemAddress address)
{
	NetworkManager::handleBikeStatusMessage(message, address);

	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)BIKE_STATUS_MESSAGE);
	bsOut.Write(message);
	//relay message to all systems except the origin of the message
	peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, address, true);
}


void ServerManager::handleFencePartMessage(fenceUpdateMessage message, RakNet::SystemAddress address)
{
	NetworkManager::handleFencePartMessage(message, address);

	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)BIKE_FENCE_PART_MESSAGE);
	bsOut.Write(message);
	//relay message to all systems except the origin of the message
	peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, address, true);
}

void ServerManager::handleGameStatusMessage(gameStatusMessage message, RakNet::SystemAddress address)
{
	NetworkManager::handleGameStatusMessage(message, address);

	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)GAME_STATUS_MESSAGE);
	bsOut.Write(message);
	//relay message to all systems except the origin of the message
	peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, address, true);
}