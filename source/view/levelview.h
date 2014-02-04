#pragma once
// OSG
#include <osg/ref_ptr>
#include <osg/Geode>
// bullet
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
		osg::ref_ptr<osg::Group> getFloor();

		void addItemBox(osg::Vec3 position);

	private:
		osg::ref_ptr<osg::Group> constructWalls(const int levelSize);
		osg::ref_ptr<osg::Group> constructFloors(const int levelSize);
		osg::ref_ptr<osg::Group> constructObstacles(const int levelSize);

		osg::ref_ptr<osg::Group> constructGroupForBoxes(std::vector<BoxModel> &boxes);
		osg::ref_ptr<osg::Group> constructRadarElementsForBoxes(std::vector<BoxModel> &boxes);
		
		osg::ref_ptr<osg::Group> m_floors;

		void setTexture(osg::ref_ptr<osg::StateSet> stateset, const std::string filePath, const int unit);

		void addShaderAndUniforms(osg::ref_ptr<osg::Node>& node, const int shaderIndex, const int levelSize, const int modelID);


		std::shared_ptr<LevelModel> m_model;
	};
}