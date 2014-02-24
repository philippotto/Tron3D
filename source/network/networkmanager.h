#pragma once
#include <stdio.h>
#include <string.h>
// OSG
#include <osg/ref_ptr>
// Qt
#include <QThread>
//raknet
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h"  // MessageID
// troen
#include "../forwarddeclarations.h"



namespace troen
{
		class NetworkManager : public QThread
		{
		public:
			NetworkManager();
			void openServer();
			virtual void run();
			void openClient();
		protected:
			RakNet::Packet *m_packet;
			RakNet::RakPeerInterface *peer;
			bool m_isServer;
			bool m_connectionAccepted;
		};


}