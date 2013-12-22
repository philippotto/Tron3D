
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
#include <osgDB/WriteFile>
#include <osg/Texture2D>
#include <osg/TexEnv>
#include <osg/TexGen>
#include <osg/TexGenNode>


#include "../model/levelmodel.h"
#include "shaders.h"

using namespace troen;

//!!!!!!!!!!!!! WARNING: AUTO_GENERATED !!!!!!!!!!!!!!!!!!!!!!
// If you want to change something generally, please edit obstacle_export.py, otherwise be sure to mark changes to this code otherwise it might be overwritten

osg::ref_ptr<osg::Geode> LevelView::autoConstructObstacles()
{
	int levelSize = m_model->getLevelSize();
	osg::ref_ptr<osg::Geode> obstacleGeode = new osg::Geode();

	
osg::ref_ptr<osg::Box> obstacle0
	= new osg::Box(osg::Vec3(0.0,40.0,0.0), 20.000009536743164, 20.000009536743164, 20.0);
osg::ref_ptr<osg::ShapeDrawable> boxDrawable0
	= new osg::ShapeDrawable(obstacle0); 
obstacleGeode->addDrawable(boxDrawable0);

osg::ref_ptr<osg::Box> obstacle1
	= new osg::Box(osg::Vec3(0.0,-40.0,0.0), 20.000009536743164, 20.000009536743164, 20.0);
osg::ref_ptr<osg::ShapeDrawable> boxDrawable1
	= new osg::ShapeDrawable(obstacle1); 
obstacleGeode->addDrawable(boxDrawable1);

	
	osg::StateSet *obstaclesStateSet = obstacleGeode->getOrCreateStateSet();
	obstaclesStateSet->ref();
	obstaclesStateSet->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::DEFAULT], osg::StateAttribute::ON);

	setTexture(obstaclesStateSet, "data/textures/troen_box_tex.tga", 0);

	osg::Uniform* textureMapU = new osg::Uniform("diffuseTexture", 0);
	obstaclesStateSet->addUniform(textureMapU);

	osg::Uniform* modelIDU = new osg::Uniform("modelID", DEFAULT);
	obstaclesStateSet->addUniform(modelIDU);

	return obstacleGeode;
}
