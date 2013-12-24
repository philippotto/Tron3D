#include "levelview.h"
// STD
#include <math.h>
// OSG
#include <osg/ImageStream>
#include <osg/Material>
#include <osg/MatrixTransform>
#include <osg/Texture2D>
#include <osg/TexMat>
#include <osg/Group>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osgDB/ReadFile>

#include <btBulletDynamicsCommon.h>

#include "../model/levelmodel.h"
#include "shaders.h"

using namespace troen;


LevelView::LevelView(std::shared_ptr<LevelModel> model)
{
	m_model = model;

	int levelSize = m_model->getLevelSize();

	m_node = new osg::Group();
	m_node->addChild(constructWalls(levelSize));
	m_node->addChild(constructFloors(levelSize));
}

osg::ref_ptr<osg::Geode> LevelView::constructWalls(int levelSize)
{
	osg::ref_ptr<osg::Geode> walls = constructGeodeForBoxes(m_model->getWalls());		
	addShaderAndUniforms(walls, shaders::OUTER_WALL, levelSize);

	return walls;
}

osg::ref_ptr<osg::Geode> LevelView::constructFloors(int levelSize)
{
	osg::ref_ptr<osg::Geode> floors = constructGeodeForBoxes(m_model->getFloors());
	addShaderAndUniforms(floors, shaders::GRID, levelSize);

	return floors;
}

void LevelView::addShaderAndUniforms(osg::ref_ptr<osg::Geode>& geode, int shaderIndex, int levelSize)
{
	osg::StateSet *stateSet = geode->getOrCreateStateSet();
	stateSet->ref();

	stateSet->setAttributeAndModes(shaders::m_allShaderPrograms[shaderIndex], osg::StateAttribute::ON);
	stateSet->addUniform(new osg::Uniform("levelSize", levelSize));
	stateSet->addUniform(new osg::Uniform("modelID", DEFAULT));
}

osg::ref_ptr<osg::Geode> LevelView::constructGeodeForBoxes(std::vector<BoxModel> &boxes)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	for (int i = 0; i < boxes.size(); ++i)
	{
		btVector3 dimensions = boxes[i].dimensions;

		osg::ref_ptr<osg::Box> box
			= new osg::Box(asOsgVec3(boxes[i].center), dimensions.x(), dimensions.y(), dimensions.z());

		osg::ref_ptr<osg::ShapeDrawable> wallDrawable
			= new osg::ShapeDrawable(box);

		geode->addDrawable(wallDrawable);
	}

	return geode;
}
