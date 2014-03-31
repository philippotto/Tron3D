#pragma once
// OSG
#include <osg/ref_ptr>
// troen
#include "pollingdevice.h"
#include "bikeinputstate.h"
#include "../forwarddeclarations.h"
#include "../network/networkmanager.h"


namespace troen
{
	namespace input
	{
	/*! The RemotePlayer class is a representation of an enemy which plays via network.*/
		class RemotePlayer
		{
		public:
			RemotePlayer(osg::ref_ptr<BikeInputState> bikeInputState);
			void update(troen::networking::bikeUpdateMessage updateMessage);
			void addNewFencePosition(btTransform fencePiece);
			bool tryGetFencePiece(btTransform &result);
		protected:
			osg::ref_ptr<BikeInputState> m_bikeInputState;
			bool m_newPosition;
			std::shared_ptr<QQueue<btTransform>> m_newFencePositions;
			std::shared_ptr<QMutex> m_fenceBufferMutex;
		};
	}
}