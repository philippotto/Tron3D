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

	private:
		void initialize();
		osg::ref_ptr<osg::Geode> constructGround();

		std::shared_ptr<LevelModel> m_model;

	};
}