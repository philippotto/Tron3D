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
		LevelView(std::shared_ptr<LevelModel> model);

	private:
		void initialize();
		osg::ref_ptr<osg::Geode> constructGround();
		osg::ref_ptr<osg::Geode> constructBox(osg::Vec3 position , osg::Vec3 scale);
		osg::ref_ptr<osg::Geode> constructObstacles();
		osg::ref_ptr<osg::Geode> LevelView::autoConstructObstacles();
		void setTexture(osg::ref_ptr<osg::StateSet> stateset, std::string filePath, int unit);

		std::shared_ptr<LevelModel> m_model;

	};
}