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
			float x, y, z;
			float quat_x, quat_y, quat_z, quat_w;
			float turnAngle, acceleration; //other ideas: often send turnAngle,acceleration, sometimes send position,rotation, linear velocity,angular velocity
		};


		class NetworkManager : public QThread
		{
		public:
			NetworkManager();
			void openServer();
			virtual void run();
			void openClient();
			void sendData();
			void enqueueMessage(bikeUpdateMessage message);
			void registerRemotePlayer(input::RemotePlayer *remotePlayer);
			bool isValidSession();
			void setLocalBikeController(troen::BikeController *controller);
			void update(long double g_gameTime);
		protected:
			RakNet::Packet *m_packet;
			RakNet::RakPeerInterface *peer;
			bool m_isServer;
			bool m_connectedToServer;
			bool m_clientsConnected;
			QQueue<bikeUpdateMessage> *m_sendMessagesQueue;
			std::vector<input::RemotePlayer*> m_remotePlayers;
			QMutex* m_sendBufferMutex;
			BikeController *m_localBikeController;
			long double m_lastUpdateTime;
		};
	}



}