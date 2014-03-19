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
#include "../controller/bikecontroller.h"
#include "qstring.h"


namespace troen
{

	namespace networking{

		class NetworkPlayerInfo;

		enum GameMessages
		{
			//game initiation
			GAME_INIT_PARAMETERS = ID_USER_PACKET_ENUM + 1,
			ADD_PLAYER = ID_USER_PACKET_ENUM+2,
			REGISTER_PLAYER_AT_SERVER = ID_USER_PACKET_ENUM + 3,
			GAME_START_MESSAGE = ID_USER_PACKET_ENUM + 4,
			//bike messages
			BIKE_POSITION_MESSSAGE = ID_USER_PACKET_ENUM + 5,
			BIKE_STATUS_MESSAGE = ID_USER_PACKET_ENUM + 6,
			BIKE_FENCE_PART_MESSAGE = ID_USER_PACKET_ENUM + 7,
			//game messages
			GAME_STATUS_MESSAGE = ID_USER_PACKET_ENUM + 8
		};
		enum gameStatus { PLAYER_DEATH_ON_WALL, PLAYER_DEATH_ON_FENCE};

		enum NETWORK_BIKESTATE
		{
			DRIVING,
			RESPAWN,
			WAITING_FOR_GAMESTART,
			INITIALIZING
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
			NETWORK_BIKESTATE status;
		};

		struct gameStatusMessage
		{
			int bikeID;
			gameStatus status;
			int value;
			short bikeID2; //if interaction of two bikes/fences are invovled
		};



		struct bikeInputUpdateMessage
		{
			int bikeID;
			float turnAngle, acceleration; //other ideas: often send turnAngle,acceleration, sometimes send position,rotation, linear velocity,angular velocity
		};

		struct fenceUpdateMessage
		{
			int bikeID;
			btTransform fencePart;
		};


		

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
			void enqueueMessage(gameStatusMessage message);
			void enqueueMessage(fenceUpdateMessage message);
			void enqueueMessage(bikeStatusMessage message);

			//sending data
			void sendUpdateMessages();
			template <typename TQueue, typename TSendStruct> 
			void sendMessages(QQueue<TQueue> *sendBufferQueue, TSendStruct &messageToSend, int order, int statusMessage);
			void sendPoints(int pointCount, int status, short secondBike = NULL);
			void synchronizeGameStart(troen::GameConfig &config);
			void setLocalGameReady();
			void sendGameStatusMessage(gameStatus status, troen::Player *bikePlayer, troen::Player *fencePlayer);
			//receiving data
			template <typename T>
			void readMessage(RakNet::Packet *packet, T& readInto);
			virtual void handleBikeStatusMessage(bikeStatusMessage message, RakNet::SystemAddress adress);
			virtual void handleBikePositionMessage(bikeUpdateMessage message, RakNet::SystemAddress adress);
			virtual void handleFencePartMessage(fenceUpdateMessage message, RakNet::SystemAddress adress);
			virtual void handleGameStatusMessage(gameStatusMessage message, RakNet::SystemAddress adress);
			virtual void addPlayer(RakNet::Packet *packet);

			//synchronize threads
			void waitOnAllPlayers();
			
			//register
			int registerRemotePlayerInput(int playerID, std::shared_ptr<input::RemotePlayer> remotePlayer);
			void registerLocalPlayer(troen::Player* player);

			//getters
			int getGameID()  { return m_gameID; }
			btTransform getStartPosition()  { return m_startPosition; }
			std::string getClientAddress();
			QColor getPlayerColor(int playerID);
			std::shared_ptr<NetworkPlayerInfo> getPlayerWithID(int bikeID);
			std::shared_ptr<NetworkPlayerInfo> getPlayerWithName(QString name);

			//data polling
			void update(long double g_gameTime);
			QQueue<gameStatusMessage> *m_receivedGameStatusMessages;
			//data pushing
			void updateFencePart(btTransform fencePart, int bikeID);

		
		signals:
			void remoteStartCall();
			void newNetworkPlayer(QString name);

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
			QQueue<fenceUpdateMessage> *m_sendFenceUpdateMessagesQueue;
			QQueue<gameStatusMessage> *m_sendGameStatusMessage;
			QQueue<bikeStatusMessage> *m_sendBikeStatusMessage;
			QMutex* m_sendBufferMutex;
			

			//players
			std::vector<std::shared_ptr<NetworkPlayerInfo>> m_players;
			std::shared_ptr<troen::Player>  m_localPlayer;
			std::shared_ptr<BikeController> m_localBikeController;
			std::shared_ptr<BikeModel> m_localBikeModel;


			long double m_lastUpdateTime;
			
			//networked information
			bool m_gameInitStarted;
			btTransform m_startPosition;
			int m_gameID;
			std::shared_ptr<NetworkPlayerInfo> m_ownPlayerInfo;
			bool m_gameStarted;
			struct bikeUpdateMessage receivedUpdateMessage, lastSentMessage, messageToSend;
			struct bikeStatusMessage receivedBikeStatusMessage, bikeStatusMessageToSend;
			struct gameStatusMessage receivedGameStatusMessage, gameStatusMessageToSend;
			struct fenceUpdateMessage receivedFenceMessage, fenceMessageToSend;
		};


		class NetworkPlayerInfo
		{
		public:
			NetworkPlayerInfo(){};
			NetworkPlayerInfo(QString name, QColor color, int networkID, bool remote, btTransform position);
		
			
			QString name;
			QColor color;
			int networkID;
			btTransform position;
			bool remote;
			int score;

			void serialize(RakNet::BitStream *bs);
			void setParametersFromRemote(RakNet::Packet *packet);
			void serializeStatus(RakNet::BitStream *bs);
			std::shared_ptr<troen::input::RemotePlayer> m_remoteInputPlayer;

			NETWORK_BIKESTATE status;

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