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
#include "qcolor.h"

//bullet
#include <btBulletDynamicsCommon.h>
// troen
#include "../forwarddeclarations.h"
#include "../input/remoteplayer.h"
#include "../troengame.h"


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

			//subclass responibilities
			virtual bool isValidSession();
			virtual void handleSubClassMessages(RakNet::Packet *packet) = NULL; //pure virtual function
			
			//queues
			void enqueueMessage(bikeUpdateMessage message);
			void enqueueMessage(bikeStatusMessage message);
			void enqueueMessage(btTransform message);
			
			//sending data
			void sendData();
			template <typename TQueue, typename TSendStruct> 
			void sendMessages(QQueue<TQueue> *sendBufferQueue, TSendStruct &messageToSend, int order, int statusMessage);
			void sendPoints(int pointCount, int status, short secondBike = NULL);
			void synchronizeGameStart(troen::GameConfig &config);

			//receiving data
			template <typename T>
			void readMessage(RakNet::Packet *packet, T& readInto);
			void receiveStatusMessage(bikeStatusMessage message);
			
			//register
			void registerRemotePlayerInput(std::shared_ptr<input::RemotePlayer> remotePlayer);
			void registerLocalPlayer(troen::Player* player);

			//getters
			int getGameID()  { return m_gameID; }
			btTransform getStartPosition()  { return m_startPosition; }
			std::string getClientAddress();
			QColor getPlayerColor(int playerID);
			std::shared_ptr<NetworkPlayerInfo> getPlayerWithID(int bikeID);

			//data polling
			void update(long double g_gameTime);


		
		signals:
			void remoteStartCall();
		public slots:
			void buildOwnPlayerInfo(const troen::GameConfig& config);

		protected:
			TroenGame *m_troenGame;

			//RakNet
			std::string m_clientAddress;
			RakNet::Packet *m_packet;
			RakNet::RakPeerInterface *peer;

			bool m_isServer;
			bool m_connectedToServer;
			short m_numClientsConnected;
			//Queues
			QQueue<bikeUpdateMessage> *m_sendUpdateMessagesQueue;
			QQueue<btTransform> *m_sendFenceUpdateMessagesQueue;
			QQueue<bikeStatusMessage> *m_sendStatusUpdateMessage;
			QMutex* m_sendBufferMutex;

			//players
			std::vector<std::shared_ptr<NetworkPlayerInfo>> m_players;
			std::shared_ptr<troen::Player>  m_localPlayer;
			std::shared_ptr<BikeController> m_localBikeController;
			std::shared_ptr<BikeModel> m_localBikeModel;


			long double m_lastUpdateTime;
			
			//networked information
			bool m_gameStarted;
			btTransform receivedFencePart,fencePartToSend;
			btTransform m_startPosition;
			int m_gameID;
			std::shared_ptr<NetworkPlayerInfo> m_ownPlayerInfo;

			struct bikeUpdateMessage receivedUpdateMessage, lastSentMessage, messageToSend;
			struct bikeStatusMessage receivedStatusMessage, statusMessageToSend;
		};


		class NetworkPlayerInfo
		{
		public:
			NetworkPlayerInfo(){};
			NetworkPlayerInfo(QString name, QColor color, int networkID, bool remote);
			QString name;
			QColor color;
			int networkID;
			bool remote;
			void serialize(RakNet::BitStream &bs);
			void setParametersFromRemote(RakNet::BitStream &bs);
			std::shared_ptr<input::RemotePlayer> m_remoteInputPlayer;
			
		
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






}