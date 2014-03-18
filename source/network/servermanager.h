#pragma once
#include "networkmanager.h"
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

		class ServerManager : public NetworkManager
		{
			Q_OBJECT
		public:
			ServerManager(TroenGame *game, QString playerName);
			void openServer();
			bool isValidSession();

			std::string getClientAddress();
			void giveIDtoClient(RakNet::Packet *packet);
			virtual void handleSubClassMessages(RakNet::Packet *packet);
			void registerClient(RakNet::Packet *packet);
		protected:
			std::string m_clientAddress;
			bool m_isServer;
			int m_numClientsConnected;


			std::shared_ptr<std::vector<btTransform>> m_startPositions;
			QString m_playerName;
		};
	}


}