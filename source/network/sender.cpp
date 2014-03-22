#include "sender.h"
#include "../constants.h"
// troen

using namespace troen;


enum GameMessages
{
	ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1
};



Sender::Sender()
{

}

void Sender::run()
{
	// subclass responsibility
	m_running = true;
	RakNet::Packet *packet = new RakNet::Packet();
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	while (m_running)
	{

		printf("Sending.\n");

		// Use a BitStream to write a custom user message
		// Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
		myVector.x = 1.0;
		myVector.y = 0.0;
		myVector.z = 99.0;
		bsOut.Write(myVector);
		bsOut.Write("Hello world");
		peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

		this->msleep(1000);




	}
	RakNet::RakPeerInterface::DestroyInstance(peer);
}

void Sender::stop(){
	m_running = false;
}