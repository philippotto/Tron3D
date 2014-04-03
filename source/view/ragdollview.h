#pragma once
// OSG
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/MatrixTransform>
// troen
#include "../forwarddeclarations.h"
#include "abstractview.h"
#include "../controller/itemcontroller.h"

namespace troen
{
	class RagDollView : public AbstractView
	{
	public:
		RagDollView(osg::Vec3 dimensions, osg::Vec3 position, LevelView*  levelView);
		void remove();
	private:
		void setTexture(osg::ref_ptr<osg::StateSet> stateset, std::string filePath, int unit);
		osg::ref_ptr<osg::MatrixTransform> m_matrixTransform;
		LevelView* m_levelView;
	};
}