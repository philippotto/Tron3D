#pragma once
//rakNet
#include "BitStream.h"
#include "RakSleep.h"
#include "RakPeerInterface.h"
#include "RakNetTypes.h"  // MessageID
// OSG
#include <osg/ref_ptr>
#include <osg/Vec3>
// Qt
#include <QThread>
#include <vector>
// troen
#include "../forwarddeclarations.h"
#include "../input/remoteplayer.h"



namespace troen
{

	namespace networking{



		struct bikeUpdateMessage
		{
			short bikeID;
			float x, y, z;
			float quat_x, quat_y, quat_z, quat_w;
			float linearVelX, linearVelY, linearVelZ;
			float angularVelZ;
		};

		struct bikeStatusMessage
		{
			short bikeID;
			int status;
			int value;
			short bikeID2; //if interaction of two bikes/fences are invovled
		};


		struct bikeInputUpdateMessage
		{
			short bikeID;
			float turnAngle, acceleration; //other ideas: often send turnAngle,acceleration, sometimes send position,rotation, linear velocity,angular velocity
		};

		enum gameStatus { PLAYER_DEATH_ON_WALL, PLAYER_DEATH_ON_OWN_FENCE, PLAYER_DEATH_ON_OTHER_PLAYER};


		class NetworkManager : public QThread
		{
			Q_OBJECT
		public:
			NetworkManager(TroenGame *game);
			void openServer();
			virtual void run();
			void openClient(std::string connectAddr);
			void sendData();
			void enqueueMessage(bikeUpdateMessage message);
			void enqueueMessage(bikeInputUpdateMessage message);
			void enqueueMessage(bikeStatusMessage message);
			void registerRemotePlayer(input::RemotePlayer *remotePlayer);
			void registerRemotePlayer(std::shared_ptr<input::RemotePlayer> remotePlayer);
			bool isValidSession();
			void registerLocalBikeController(std::shared_ptr<troen::BikeController> controller);
			void update(long double g_gameTime);

			void sendStatusUpdateMessage(int message);
			void sendPoints(int pointCount, int status, short secondBike = NULL);
			void receiveStatusMessage(bikeStatusMessage message);
			std::string getClientAddress();
			void synchronizeGameStart();
		
		signals:
			void remoteStartCall();

		protected:
			std::string m_clientAddress;
			RakNet::Packet *m_packet;
			RakNet::RakPeerInterface *peer;
			bool m_isServer;
			bool m_connectedToServer;
			bool m_numClientsConnected;
			QQueue<bikeUpdateMessage> *m_sendUpdateMessagesQueue;
			QQueue<bikeInputUpdateMessage> *m_sendInputUpdateMessagesQueue;
			QQueue<bikeStatusMessage> *m_sendStatusUpdateMessage;
			std::vector<std::shared_ptr<input::RemotePlayer>> m_remotePlayers;
			QMutex* m_sendBufferMutex;
			std::shared_ptr<BikeController> m_localBikeController;
			long double m_lastUpdateTime;
			short m_gameID;
			TroenGame *m_troenGame;
			bool m_gameStarted;
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