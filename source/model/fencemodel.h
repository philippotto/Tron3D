#pragma once
// troen
#include "../forwarddeclarations.h"
#include "abstractmodel.h"

namespace troen
{
	class FenceModel : public AbstractModel
	{
	public:
		FenceModel(FenceController* fenceController);
		void addFence();
		void addFencePart(btVector3 a, btVector3 b);
		float getFenceHeight();

		btRigidBody* FenceModel::getLastPart();
		void removeAllFences();

		void addFenceMarker(btVector3 a);
	private:
		FenceController* m_fenceController;
	};
}