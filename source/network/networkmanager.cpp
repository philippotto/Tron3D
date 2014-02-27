#include "networkmanager.h"
#include "../constants.h"

#include <stdio.h>
#include <string.h>

#include "MessageIdentifiers.h"


//
#include <QQueue>

//raknet


using namespace troen::networking;
#define MAX_CLIENTS 10
#define SERVER_PORT 60000


enum GameMessages
{
	BIKE_POSITION_MESSSAGE = ID_USER_PACKET_ENUM + 1
};

struct bikeUpdateMessage updateMessage;

NetworkManager::NetworkManager()
{
	m_packet = new RakNet::Packet;
	peer = RakNet::RakPeerInterface::GetInstance();
	m_connectedToServer = false;
	m_clientsConnected = false;
	m_sendMessagesQueue = new QQueue<bikeUpdateMessage>();
	m_remotePlayers = std::vector<input::RemotePlayer*>();
	m_sendBufferMutex = new QMutex();
}

void  NetworkManager::enqueueMessage(osg::Vec3 position, float angle, float acceleration)
{
	bikeUpdateMessage update = { position.x(), position.y(), position.z(),angle,acceleration};
	m_sendBufferMutex->lock();
	m_sendMessagesQueue->enqueue(update);
	m_sendBufferMutex->unlock();
}


void NetworkManager::registerRemotePlayer(troen::input::RemotePlayer *remotePlayer)
{
	m_remotePlayers.push_back(remotePlayer);
}

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
				m_connectedToServer = true;
				break;
			case ID_NEW_INCOMING_CONNECTION:
				m_clientsConnected = true;
				printf("A connection is incoming.\n");
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
				RakNet::RakString rs;
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(updateMessage);
				std::cout << updateMessage.x << " " << updateMessage.y << " " << updateMessage.z << " " << updateMessage.turnAngle << " " << updateMessage.acceleration << std::endl;
				m_remotePlayers[0]->update(updateMessage);
			}
				break;

			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}
		}


		sendData();
		
	}

	//cleanup
	RakNet::RakPeerInterface::DestroyInstance(peer);
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

void NetworkManager::sendData()
{
	while (!m_sendMessagesQueue->empty())
	{
		if (m_connectedToServer || m_clientsConnected)
		{

			RakNet::BitStream bsOut;
			// Use a BitStream to write a custom user message
			//Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically
			bsOut.Write((RakNet::MessageID)BIKE_POSITION_MESSSAGE);
			m_sendBufferMutex->lock();
			updateMessage = m_sendMessagesQueue->dequeue();
			m_sendBufferMutex->unlock();
			bsOut.Write(updateMessage);
			
			//peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
			peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		}
	}
}

void NetworkManager::openClient()
{
	m_isServer = false;
	RakNet::SocketDescriptor sd;
	peer->Startup(1, &sd, 1);

	char str[512];

	printf("Enter server IP or hit enter for 127.0.0.1\n");
	//gets(str);
	std::cin.getline(str, 510);
	if (str[0] == 0){
		strncpy_s(str, "127.0.0.1",10);
	}
	printf("Starting the client.\n");
	RakNet::ConnectionAttemptResult r= peer->Connect(str, SERVER_PORT, 0, 0);

	//calls the run method in a seperate thread
	start();
}


bool NetworkManager::isValidSession()
{
	//for now
	return m_connectedToServer || m_clientsConnected;
}