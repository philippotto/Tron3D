#pragma once
// OSG
#include <osg/ref_ptr>
#include <osg/Geode>
// troen
#include "../forwarddeclarations.h"
#include "abstractview.h"

namespace troen
{
	class LevelView : public AbstractView
	{
	public:
		LevelView();
		
		osg::ref_ptr<osg::Group> getNode();

	private:
		void initialize();
		osg::ref_ptr<osg::Geode> constructGround();
		osg::ref_ptr<osg::Group> m_node;


	};
}