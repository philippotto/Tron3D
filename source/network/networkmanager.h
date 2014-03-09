#pragma once
//rakNet
#include "BitStream.h"
#include "RakSleep.h"
#include "RakPeerInterface.h"
#include "RakNetTypes.h"  // MessageID
#include "MessageIdentifiers.h"
// OSG
#include <osg/ref_ptr>
#include <osg/Vec3>
// Qt
#include <QThread>
#include <vector>

//bullet
#include <btBulletDynamicsCommon.h>
// troen
#include "../forwarddeclarations.h"
#include "../input/remoteplayer.h"



namespace troen
{

	namespace networking{

		enum GameMessages
		{
			BIKE_POSITION_MESSSAGE = ID_USER_PACKET_ENUM + 1,
			GAME_START_MESSAGE = ID_USER_PACKET_ENUM + 2,
			BIKE_STATUS_MESSAGE = ID_USER_PACKET_ENUM + 3,
			GAME_INIT_PARAMETERS = ID_USER_PACKET_ENUM + 4,
			BIKE_FENCE_PART_MESSAGE = ID_USER_PACKET_ENUM + 5
		};

		struct bikeUpdateMessage
		{
			int bikeID;
			float x, y, z;
			float quat_x, quat_y, quat_z, quat_w;
			float linearVelX, linearVelY, linearVelZ;
		};

		struct bikeStatusMessage
		{
			int bikeID;
			int status;
			int value;
			short bikeID2; //if interaction of two bikes/fences are invovled
		};


		struct bikeInputUpdateMessage
		{
			int bikeID;
			float turnAngle, acceleration; //other ideas: often send turnAngle,acceleration, sometimes send position,rotation, linear velocity,angular velocity
		};

		enum gameStatus { PLAYER_DEATH_ON_WALL, PLAYER_DEATH_ON_OWN_FENCE, PLAYER_DEATH_ON_OTHER_PLAYER};
		

		class NetworkManager : public QThread
		{
			Q_OBJECT
		public:
			NetworkManager(TroenGame *game);
			virtual void run();
			virtual bool isValidSession();
			void sendData();
			void enqueueMessage(bikeUpdateMessage message);
			void enqueueMessage(bikeStatusMessage message);
			void enqueueMessage(btTransform message);
			void registerRemotePlayer(input::RemotePlayer *remotePlayer);
			void registerRemotePlayer(std::shared_ptr<input::RemotePlayer> remotePlayer);
			void registerLocalBikeController(std::shared_ptr<troen::BikeController> controller);
			void update(long double g_gameTime);

			void sendStatusUpdateMessage(int message);
			void sendPoints(int pointCount, int status, short secondBike = NULL);
			void receiveStatusMessage(bikeStatusMessage message);
			std::string getClientAddress();
			void synchronizeGameStart();
			int getGameID()  { return m_gameID; }
			btTransform getStartPosition()  { return m_startPosition; }
			template <typename TQueue, typename TSendStruct> 
			void sendMessages(QQueue<TQueue> *sendBufferQueue, TSendStruct &messageToSend, int order, int statusMessage);

		
		signals:
			void remoteStartCall();
		protected:
			virtual void handleSubClassMessages(RakNet::Packet *packet) = NULL; //pure virtual function

			std::string m_clientAddress;
			RakNet::Packet *m_packet;
			RakNet::RakPeerInterface *peer;
			bool m_isServer;
			bool m_connectedToServer;
			short m_numClientsConnected;
			QQueue<bikeUpdateMessage> *m_sendUpdateMessagesQueue;
			QQueue<btTransform> *m_sendFenceUpdateMessagesQueue;
			QQueue<bikeStatusMessage> *m_sendStatusUpdateMessage;
			std::vector<std::shared_ptr<input::RemotePlayer>> m_remotePlayers;
			QMutex* m_sendBufferMutex;
			std::shared_ptr<BikeController> m_localBikeController;
			long double m_lastUpdateTime;
			int m_gameID;
			btTransform m_startPosition;

			TroenGame *m_troenGame;
			bool m_gameStarted;

			struct bikeUpdateMessage receivedUpdateMessage, lastSentMessage, messageToSend;
			struct bikeStatusMessage receivedStatusMessage, statusMessageToSend;
			btTransform receivedFencePart,fencePartToSend;
		};
	}

	template <typename T>
	bool is_in(const T& val, const std::initializer_list<T>& list)
	{
		for (const auto& i : list) {
			if (val == i) {
				return true;
			}
		}
		return false;
	}

	template <typename T>
	void readMessage(RakNet::Packet *packet, T& readInto)
	{
		RakNet::BitStream bsIn(packet->data, packet->length, false);
		bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
		bsIn.Read(readInto);
	}




}