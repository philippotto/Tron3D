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

		class ClientManager : public NetworkManager
		{
			Q_OBJECT
		public:
			ClientManager(TroenGame *game);
			virtual void run();
			void openClient(std::string connectAddr);

			bool isValidSession();
			void setInitParameters(RakNet::Packet *packet);

		protected:
	
			bool m_isServer;
			bool m_connectedToServer;
		};
	}


}