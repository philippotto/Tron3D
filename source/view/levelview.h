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

		void addItemBox(osg::Vec3 position);

	private:
		osg::ref_ptr<osg::Group> constructWalls(int levelSize);
		osg::ref_ptr<osg::Group> constructFloors(int levelSize);
		osg::ref_ptr<osg::Group> constructObstacles(int levelSize);

		osg::ref_ptr<osg::Group> constructGroupForBoxes(std::vector<BoxModel> &boxes);
		osg::ref_ptr<osg::Group> constructRadarElementsForBoxes(std::vector<BoxModel> &boxes);

		void setTexture(osg::ref_ptr<osg::StateSet> stateset, std::string filePath, int unit);

		void addShaderAndUniforms(osg::ref_ptr<osg::Group>& group, int shaderIndex, int levelSize);

		std::shared_ptr<LevelModel> m_model;
	};
}