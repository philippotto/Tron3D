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
		class Sender : public QThread
		{
		public:
			Sender();
			virtual void run();
			void stop();

			struct MyVector
			{
				float x, y, z;
			} myVector, myVector1;
		protected:
			bool m_running;
		};



}