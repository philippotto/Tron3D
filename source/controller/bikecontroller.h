#pragma once

#include "abstractcontroller.h"
#include <osg/Referenced>
#include "../forwarddeclarations.h"

using namespace troen::input;

namespace troen
{

	class BikeController : public AbstractController, public osg::Referenced
	{
	public:
		BikeController();

	};
}