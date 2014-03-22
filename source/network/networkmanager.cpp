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

#include "networkmanager.h"
#include <thread>
//raknet
#include "GetTime.h"


using namespace troen::networking;
#define MAX_CLIENTS 10
#define SERVER_PORT 60000




NetworkManager::NetworkManager(troen::TroenGame *game)
{
	m_packet = new RakNet::Packet;
	peer = RakNet::RakPeerInterface::GetInstance();
	m_sendUpdateMessagesQueue = new QQueue<bikeUpdateMessage>();
	m_sendFenceUpdateMessagesQueue = new QQueue<fenceUpdateMessage>();
	m_sendGameStatusMessage = new QQueue<gameStatusMessage>();
	m_receivedGameStatusMessages = new QQueue<gameStatusMessage>();
	m_sendBikeStatusMessage = new QQueue<bikeStatusMessage>();
	m_players = std::vector<std::shared_ptr<NetworkPlayerInfo>>();
	m_sendBufferMutex = new QMutex();
	
	m_localBikeControllers = std::vector<std::shared_ptr<BikeController>>();
	m_lastUpdateTime = 0;
	m_gameID = 0;
	m_troenGame = game;
	//game init started when player presses start button
	m_gameInitStarted = false;
	//game started when all players have finished initalizing
	m_gameStarted = false;
	
}



bool NetworkManager::isValidSession()
{
	//sublcass responsibilty
	return false;
}


////////////////////////////////////////////////////////////////////////////////
//
// updating / raknet thread
//
////////////////////////////////////////////////////////////////////////////////

void NetworkManager::run()
{
	RakNet::Packet *packet;
	while (1)
	{
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			switch (packet->data[0])
			{
			case BIKE_POSITION_MESSSAGE:
			{
										   readMessage(packet, receivedUpdateMessage);
										   handleBikePositionMessage(receivedUpdateMessage, packet->systemAddress);
			}
				break;

			case BIKE_STATUS_MESSAGE:
			{
										readMessage(packet, receivedBikeStatusMessage);
										handleBikeStatusMessage(receivedBikeStatusMessage, packet->systemAddress);

			}
				break;

			case BIKE_FENCE_PART_MESSAGE:
			{

											readMessage(packet, receivedFenceMessage);
											handleFencePartMessage(receivedFenceMessage, packet->systemAddress);
			}

			case GAME_START_MESSAGE:
			{

									   //prevent game from calling start two times due to receviment of own packet
									   if (!m_gameInitStarted)
									   {

										   emit remoteStartCall();
										   m_gameInitStarted = true;
									   }
			}
				break;

			case ADD_PLAYER:
			{
							   addPlayer(packet);
			}
				break;

			case GAME_STATUS_MESSAGE:
			{
										readMessage(packet, receivedGameStatusMessage);
										handleGameStatusMessage(receivedGameStatusMessage, packet->systemAddress);
		
			}
				break;
			case CHANGE_LEVEL:
			{
								 changeLevel(packet);
			}
				break;

			default:
			{
					   //handle all the client or server related stuff
					   handleSubClassMessages(packet);
			}
				break;



			}

		}
		sendUpdateMessages();
		this->msleep(10);
	}

	//cleanup
	RakNet::RakPeerInterface::DestroyInstance(peer);

}

////////////////////////////////////////////////////////////////////////////////
//
// synchronizing / receiving (raknet thread)
//
////////////////////////////////////////////////////////////////////////////////

bool troen::networking::NetworkManager::addPlayer(RakNet::Packet *packet)
{
	//instantiate the remote player
	std::shared_ptr<NetworkPlayerInfo> remote_player = std::make_shared<NetworkPlayerInfo>();
	remote_player->setParametersFromRemote(packet);
	
	if (getPlayerWithID(remote_player->networkID) != NULL)  //player was added before
	{
		remote_player.reset();
		return false;
	}

	if ( getPlayerWithName(remote_player->name) != NULL) //other player already has name
	{
		remote_player.reset();
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)PLAYERNAME_REFUSED);
		//notify sending client
		peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, packet->systemAddress, false);
		return false;
	}

	m_players.push_back(remote_player);
	std::cout << "got remote player: " << remote_player->name.toStdString() << std::endl;


	//add player in UI
	emit newNetworkPlayer(remote_player->name);

	return true;
}


void NetworkManager::handleBikeStatusMessage(bikeStatusMessage message, RakNet::SystemAddress adress)
{
	std::cout << "bike status message" << std::endl;
	if (m_players.size() > 1)
		getPlayerWithID(message.bikeID)->status = message.status;
}

void NetworkManager::handleBikePositionMessage(bikeUpdateMessage message, RakNet::SystemAddress adress)
{
	if (m_players.size() > 1)
		getPlayerWithID(receivedUpdateMessage.bikeID)->m_remoteInputPlayer->update(receivedUpdateMessage);
}

void NetworkManager::handleFencePartMessage(fenceUpdateMessage message, RakNet::SystemAddress adress)
{
	if (m_players.size() > 1)
		getPlayerWithID(receivedFenceMessage.bikeID)->m_remoteInputPlayer->addNewFencePosition(receivedFenceMessage.fencePart);
}

void NetworkManager::handleGameStatusMessage(gameStatusMessage message, RakNet::SystemAddress adress)
{
	m_receivedGameStatusMessages->push_back(message);
	std::cout << "game status " << receivedGameStatusMessage.status << std::endl;
}


void NetworkManager::changeLevel(RakNet::Packet *packet)
{
	int levelID;
	readMessage(packet, levelID);

	emit levelChanged(levelID);
}



////////////////////////////////////////////////////////////////////////////////
//
// synchronizing / sending (raknet thread)
//
////////////////////////////////////////////////////////////////////////////////

void NetworkManager::setLocalGameReady()
{
	getPlayerWithID(m_gameID)->status = WAITING_FOR_GAMESTART;

	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)BIKE_STATUS_MESSAGE);
	getPlayerWithID(m_gameID)->serializeStatus(&bsOut);
	peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}


void NetworkManager::synchronizeGameStart(troen::GameConfig &config)
{
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)GAME_START_MESSAGE);
	peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	m_gameInitStarted = true;
}

void NetworkManager::synchronizeLevel(int levelID)
{
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)CHANGE_LEVEL);
	bsOut.Write(levelID);
	peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}



////////////////////////////////////////////////////////////////////////////////
//
// sending methods / raknet thread
//
////////////////////////////////////////////////////////////////////////////////

void NetworkManager::sendUpdateMessages()
{
	if (isValidSession() && m_gameStarted)
	{
		sendMessages(m_sendUpdateMessagesQueue, messageToSend, UNRELIABLE_SEQUENCED, BIKE_POSITION_MESSSAGE);
		sendMessages(m_sendFenceUpdateMessagesQueue, fenceMessageToSend, RELIABLE_SEQUENCED, BIKE_FENCE_PART_MESSAGE);
		sendMessages(m_sendGameStatusMessage, gameStatusMessageToSend, RELIABLE_SEQUENCED, GAME_STATUS_MESSAGE);
		sendMessages(m_sendBikeStatusMessage, bikeStatusMessageToSend, RELIABLE_SEQUENCED, BIKE_STATUS_MESSAGE);

	}
}

template <typename TQueue, typename TSendStruct>
void NetworkManager::sendMessages(QQueue<TQueue> *sendBufferQueue, TSendStruct &messageToSend, int order, int statusMessage)
{

	while (!sendBufferQueue->empty())
	{
		RakNet::BitStream bsOut;
		// Use a BitStream to write a custom user message
		//Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically
		bsOut.Write((RakNet::MessageID)static_cast<GameMessages>(statusMessage));
		m_sendBufferMutex->lock();
		messageToSend = sendBufferQueue->dequeue();
		m_sendBufferMutex->unlock();
		bsOut.Write(messageToSend);

		peer->Send(&bsOut, HIGH_PRIORITY, static_cast<PacketReliability>(order), 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	}
}


////////////////////////////////////////////////////////////////////////////////
//
// helpers
//
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void NetworkManager::readMessage(RakNet::Packet *packet, T& readInto)
{
	RakNet::BitStream bsIn(packet->data, packet->length, false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	bsIn.Read(readInto);
}


inline std::shared_ptr<NetworkPlayerInfo> NetworkManager::getPlayerWithID(int bikeID)
{
	for (auto player : m_players)
	{
		if (player->networkID == bikeID)
			return player;
	}
	return NULL;
}

std::shared_ptr<NetworkPlayerInfo> NetworkManager::getPlayerWithName(QString name)
{
	for (auto player : m_players)
	{
		if (player->name == name)
			return player;
	}
	return NULL;
}

QColor NetworkManager::getPlayerColor(int playerID)
{
	//later, we might actually synchronize the chosen color
	return std::vector<QColor>{ QColor(255.0, 0.0, 0.0), QColor(0.0, 255.0, 0.0), QColor(0.0, 0.0, 255.0),
		QColor(255.0, 255.0, 0.0), QColor(0.0, 255.0, 255.0), QColor(255.0, 0.0, 255.0) }[playerID];
}


////////////////////////////////////////////////////////////////////////////////
//
// updating / main thread
//
////////////////////////////////////////////////////////////////////////////////

void NetworkManager::update(long double g_gameTime)
{
	if (this->isValidSession())
	{
		pollPositionUpdates(g_gameTime);
	}
}

void NetworkManager::updateFencePart(btTransform fencePart, int bikeID)
{
	fenceUpdateMessage message = { bikeID, fencePart };
	enqueueMessage(message);
}


void NetworkManager::pollPositionUpdates(long double g_gameTime)
{
	for (auto localBikeController : m_localBikeControllers)
	{

		btTransform transform = localBikeController->getModel()->getTransform();
		btQuaternion quat = localBikeController->getModel()->getRotationQuat();
		btVector3 linearVelocity = localBikeController->getModel()->getLinearVelocity();
		bikeUpdateMessage message = {
			RakNet::GetTime(),
			localBikeController->player()->getNetworkID(),
			transform,
			linearVelocity.x(), linearVelocity.y(), linearVelocity.z()
		};

		//std::cout << "bikeID" << localBikeController->player()->getNetworkID() << " transform origin x " << transform.getOrigin().x() << " y " << transform.getOrigin().y() << " z " << transform.getOrigin().z()
		//	<< " rotation x " << transform.getRotation().getX() << " y " << transform.getRotation().getY() << " z " << transform.getRotation().getZ() << " lin vel x " << linearVelocity.x() << " y " << linearVelocity.y() << " z " << linearVelocity.z() << std::endl;

		if ((message.linearVelX != lastSentMessage.linearVelX) || (message.linearVelY != lastSentMessage.linearVelY) || (message.linearVelZ != lastSentMessage.linearVelZ) || g_gameTime - m_lastUpdateTime > 20.0)
		{
			enqueueMessage(message);
			lastSentMessage = message;
		}
	}
}



////////////////////////////////////////////////////////////////////////////////
//
// sending methods / main thread
//
////////////////////////////////////////////////////////////////////////////////

void NetworkManager::sendGameStatusMessage(gameStatus status, troen::Player *bikePlayer, troen::Player *fencePlayer)
{
	gameStatusMessage message;
	if (fencePlayer!=NULL)
		message = { bikePlayer->getNetworkID(), status, bikePlayer->points(), fencePlayer->getNetworkID() };
	else
		message = { bikePlayer->getNetworkID(), status, bikePlayer->points(), NULL };

	getPlayerWithID(bikePlayer->getNetworkID())->score = bikePlayer->points();
	enqueueMessage(message);
}


void  NetworkManager::enqueueMessage(bikeUpdateMessage message)
{
	m_sendBufferMutex->lock();
	m_sendUpdateMessagesQueue->enqueue(message);
	m_sendBufferMutex->unlock();
}

void  NetworkManager::enqueueMessage(fenceUpdateMessage message)
{
	m_sendBufferMutex->lock();
	m_sendFenceUpdateMessagesQueue->enqueue(message);
	m_sendBufferMutex->unlock();
}

void  NetworkManager::enqueueMessage(gameStatusMessage message)
{
	m_sendBufferMutex->lock();
	m_sendGameStatusMessage->enqueue(message);
	m_sendBufferMutex->unlock();
}


void  NetworkManager::enqueueMessage(bikeStatusMessage message)
{
	m_sendBufferMutex->lock();
	m_sendBikeStatusMessage->enqueue(message);
	m_sendBufferMutex->unlock();
}


////////////////////////////////////////////////////////////////////////////////
//
// synchronizing / main thread
//
////////////////////////////////////////////////////////////////////////////////

void NetworkManager::waitOnAllPlayers()
{
	//block until every player has waiting for gamestart status (aka finished initiliazing)
	bool ready = false;
	while (!ready)
	{
		ready = true;
		for (auto player : m_players)
		{
			if (player->status != WAITING_FOR_GAMESTART)
				ready = false;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

	}

	m_gameStarted = true;
}
////////////////////////////////////////////////////////////////////////////////
//
// registering & building
//
////////////////////////////////////////////////////////////////////////////////


int NetworkManager::registerRemotePlayerInput(int playerID, std::shared_ptr<troen::input::RemotePlayer> remoteInputPlayer)
{
	NetworkPlayerInfo *remotePlayer = getPlayerWithID(playerID).get();
	
	if (remotePlayer!=NULL)
		remotePlayer->m_remoteInputPlayer = remoteInputPlayer;
	
	return 1;
}

void NetworkManager::registerLocalPlayer(troen::Player* player)
{
	m_localBikeControllers.push_back(player->bikeController());
}

////////////////////////////////////////////////////////////////////////////////
//
// NetworkPlayerInfo
//
////////////////////////////////////////////////////////////////////////////////

NetworkPlayerInfo::NetworkPlayerInfo(QString name, QColor color, int networkID, bool remote, btTransform position) :
name(name), color(color), networkID(networkID), remote(remote), position(position)
{
	m_remoteInputPlayer = NULL;
	status = INITIALIZING;
	score = 0;
}



void NetworkPlayerInfo::serializeStatus(RakNet::BitStream *bs)
{
	bikeStatusMessage statusMessageToSend = { networkID, status };
	bs->Write(statusMessageToSend);
}

void NetworkPlayerInfo::serialize(RakNet::BitStream *bs)
{
	bs->Write(networkID);

	char message[32];
	strncpy_s(message, name.toStdString().c_str(), 32);
	bs->Write(message);

	bs->WriteVector(color.red(), color.green(), color.blue());

	bs->Write(position);
}

void NetworkPlayerInfo::setParametersFromRemote(RakNet::Packet *packet)
{
	RakNet::BitStream bsIn(packet->data, packet->length, false);

	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	bsIn.Read(networkID);

	char message[32];
	bsIn.Read(message);
	name = QString(message);

	int red, green, blue;
	bsIn.ReadVector(red, green, blue);
	color.setRed(red); color.setGreen(green); color.setBlue(blue);

	bsIn.Read(position);

	remote = true;


}