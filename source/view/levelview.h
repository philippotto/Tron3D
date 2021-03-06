#pragma once
// OSG
#include <osg/ref_ptr>
#include <osg/Geode>
#include "osg/StateAttribute"
// bullet
#include <btBulletDynamicsCommon.h>
// troen
#include "../forwarddeclarations.h"
#include "abstractview.h"
#include "../model/levelmodel.h"
#include "../controller/itemcontroller.h"

namespace troen
{
	class LevelView : public AbstractView
	{
	public:
		LevelView(std::shared_ptr<LevelModel> model, std::string levelName);
		void reload(std::string levelName);

		osg::ref_ptr<osg::Group> getFloor();
		void addItemBox(osg::ref_ptr<osg::MatrixTransform>& matrixTransform);
		void removeItemBox(osg::ref_ptr<osg::MatrixTransform>& matrixTransform);

		void setBendingFactor(float bendingFactor);
		void setBendingActive(bool val);
	private:
		osg::ref_ptr<osg::Group> constructFloors(const int levelSize);
		osg::ref_ptr<osg::Group> constructObstacles(const int levelSize, std::string levelName);

		osg::ref_ptr<osg::Group> constructGroupForBoxes(std::vector<BoxModel> &boxes);
		osg::ref_ptr<osg::Group> constructRadarElementsForBoxes(std::vector<BoxModel> &boxes);

		osg::ref_ptr<osg::Group> m_floors;

		void setTexture(osg::ref_ptr<osg::StateSet> stateset, std::string filePath, int unit, bool override = false);
		void addShaderAndUniforms(osg::ref_ptr<osg::Node> node, int shaderIndex, int levelSize, int modelID, float alpha, float trueColor = 0.0);
		std::shared_ptr<LevelModel> m_model;

		osg::Uniform *m_bendedUniform;
		osg::Uniform *m_bendingActiveUniform;
		osg::Group *m_itemGroup;

	};
}