#pragma once
// OSG
//bullet
// troen
#include "../forwarddeclarations.h"
#include "abstractcontroller.h"

namespace troen
{
	class ItemController : public AbstractController
	{
	public:
		ItemController();

		void triggerOn(BikeController* bikeController);
	};
}