#pragma once

#include "abstractview.h"
#include <osg/Referenced>
#include "../forwarddeclarations.h"

using namespace troen::input;

namespace troen
{

	class BikeView : public AbstractView, public osg::Referenced
	{
	public:
		BikeView();

	};
}