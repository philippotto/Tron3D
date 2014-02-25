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
			float turnAngle, acceleration;
		};

		class NetworkManager : public QThread
		{
		public:
			NetworkManager();
			void openServer();
			virtual void run();
			void openClient();
			void sendData();
			void enqueueMessage(osg::Vec3 position, float angle, float acceleration);
			void registerRemotePlayer(input::RemotePlayer *remotePlayer);
			bool isValidSession();
		protected:
			RakNet::Packet *m_packet;
			RakNet::RakPeerInterface *peer;
			bool m_isServer;
			bool m_connectedToServer;
			bool m_clientsConnected;
			QQueue<bikeUpdateMessage> *m_sendMessagesQueue;
			std::vector<input::RemotePlayer*> m_remotePlayers;
		};
	}



}