#pragma once
// OSG
#include <osg/ref_ptr>
#include <osg/Group>
// STD
#include <vector>
// troen
#include "../forwarddeclarations.h"

namespace troen
{
	class AbstractView
	{
		public:
			AbstractView();

			virtual osg::ref_ptr<osg::Group> getNode();
		
		protected:
			osg::ref_ptr<osg::Group> m_node;
	};
}