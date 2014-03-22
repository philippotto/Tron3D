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
		class Receiver : public QThread
		{
		public:
			Receiver();
			virtual void run();
			void stop();

		};

}