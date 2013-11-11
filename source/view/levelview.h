#pragma once

#include "abstractview.h"

#include <osg/ref_ptr>
#include "../forwarddeclarations.h"

namespace troen
{
	class LevelView : public AbstractView
	{
	public:
		LevelView();
		osg::ref_ptr<osg::Group> getNode();

	private:
		void initialize();
		osg::ref_ptr<osg::Group> m_node;


	};
}