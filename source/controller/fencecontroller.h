#pragma once
// OSG
#include <osg/Vec3>
//troen
#include "../forwarddeclarations.h"
#include "abstractcontroller.h"

using namespace troen;

namespace troen
{
	class FenceController : public AbstractController
	{
	public:
		FenceController();
		void update(osg::Vec3 position);

	private:
		osg::Vec3 m_lastPosition;
	};
}