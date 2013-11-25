#pragma once
// troen
#include "../forwarddeclarations.h"
#include "abstractmodel.h"

namespace troen
{
	class FenceModel : public AbstractModel
	{
	public:
		FenceModel();
		void addFence();
		void addFencePart(btVector3 a, btVector3 b);
		float getFenceHeight();

	private:
		void addFenceMarker(btVector3 a);

	};
}