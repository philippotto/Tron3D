#pragma once
// OSG
#include <osg/ref_ptr>
#include <osg/Geode>

#include <btBulletDynamicsCommon.h>

// troen
#include "../forwarddeclarations.h"
#include "abstractview.h"
#include "../model/levelmodel.h"

namespace troen
{
	class LevelView : public AbstractView
	{
	public:
		LevelView(std::shared_ptr<LevelModel> model);

	private:
		osg::ref_ptr<osg::Geode> constructWalls(int levelSize);
		osg::ref_ptr<osg::Geode> constructFloors(int levelSize);
		osg::ref_ptr<osg::Geode> constructGeodeForBoxes(std::vector<BoxModel> &boxes);

		void LevelView::addShaderAndUniforms(osg::ref_ptr<osg::Geode>& geode, int shaderIndex, int levelSize);

		std::shared_ptr<LevelModel> m_model;
		
		// TODO find a better place
		osg::Vec3 LevelView::asOsgVec3(const btVector3& v)
		{
			return osg::Vec3(v.x(), v.y(), v.z());
		};

	};
}