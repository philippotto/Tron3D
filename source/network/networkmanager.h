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
//raknet





// troen
#include "../forwarddeclarations.h"




namespace troen
{

	namespace networking{



		struct bikeUpdateMessage
		{
			float x, y, z;
		};

		class NetworkManager : public QThread
		{
		public:
			NetworkManager();
			void openServer();
			virtual void run();
			void openClient();
			void sendData();
			void enqueueMessage(osg::Vec3 position);
		protected:
			RakNet::Packet *m_packet;
			RakNet::RakPeerInterface *peer;
			bool m_isServer;
			bool m_connectionAccepted;
			QQueue<bikeUpdateMessage> *m_sendMessagesQueue;
		};
	}



}