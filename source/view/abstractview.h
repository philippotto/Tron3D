#pragma once
// OSG
#include <osg/ref_ptr>
#include <osg/Group>
// STD
#include <vector>
//osg
#include <osg/Shader>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>


// bullet
#include <btBulletDynamicsCommon.h>

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