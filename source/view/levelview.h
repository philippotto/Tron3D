#pragma once
// OSG
#include <osg/ref_ptr>
#include <osg/Geode>
// troen
#include "../forwarddeclarations.h"
#include <memory>

#include "abstractview.h"


namespace troen
{
	class LevelView : public AbstractView
	{
	public:
		LevelView(std::shared_ptr<LevelModel> model);
		
		osg::ref_ptr<osg::Group> getNode();

	private:
		void initialize();
		osg::ref_ptr<osg::Geode> constructGround();
		osg::ref_ptr<osg::Group> m_node;

		std::shared_ptr<LevelModel> m_model;

	};
}