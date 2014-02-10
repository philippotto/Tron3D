#include "levelview.h"
// STD
#include <math.h>
// OSG
#include <osg/ImageStream>
#include <osg/Material>
#include <osg/MatrixTransform>
#include <osg/Texture2D>
#include <osg/TexMat>
#include <osg/ShapeDrawable>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/Texture2D>
#include <osg/TexEnv>
#include <osg/TexGen>
#include <osg/TexGenNode>
// bullet
#include <btBulletDynamicsCommon.h>
// troen
#include "shaders.h"
#include "../constants.h"

using namespace troen;


LevelView::LevelView(std::shared_ptr<LevelModel> model) :
AbstractView()
{
	m_model = model;

	int levelSize = m_model->getLevelSize();

	m_node = new osg::Group();

	m_node->addChild(constructObstacles(levelSize));
	m_node->addChild(constructWalls(levelSize));
	m_node->addChild(constructFloors(levelSize));
}

osg::ref_ptr<osg::Group> LevelView::constructWalls(int levelSize)
{
	osg::ref_ptr<osg::Group> wallsGroup = new osg::Group();
	wallsGroup->setName("wallsGroup");

    osg::ref_ptr<osg::Group> walls = constructGroupForBoxes(m_model->getWalls());
	addShaderAndUniforms(static_cast<osg::ref_ptr<osg::Node>>(walls), shaders::OUTER_WALL, levelSize, DEFAULT);
	walls->setNodeMask(CAMERA_MASK_MAIN);
	wallsGroup->addChild(walls);

	osg::ref_ptr<osg::Group> radarWalls = constructRadarElementsForBoxes(m_model->getWalls());
	radarWalls->setNodeMask(CAMERA_MASK_RADAR);
	wallsGroup->addChild(radarWalls);

    return wallsGroup;
}

osg::ref_ptr<osg::Group> LevelView::constructFloors(int levelSize)
{
	osg::ref_ptr<osg::Group> floorsGroup = new osg::Group();
	osg::ref_ptr<osg::Group> floors = constructGroupForBoxes(m_model->getFloors());
	floors->setName("floorsNode");
	osg::StateSet *obstaclesStateSet = floors->getOrCreateStateSet();
	osg::Uniform* textureMapU = new osg::Uniform("diffuseTexture", 0);
	obstaclesStateSet->addUniform(textureMapU);
	setTexture(obstaclesStateSet, "data/textures/floor.tga", 0);

	//will be overwritten if reflection is used
	addShaderAndUniforms(static_cast<osg::ref_ptr<osg::Node>>(floors), shaders::GRID_NOREFLECTION, levelSize, GLOW);

	floors->setNodeMask(CAMERA_MASK_MAIN);
	floorsGroup->addChild(floors);


	osg::ref_ptr<osg::Group> radarFloors = constructRadarElementsForBoxes(m_model->getFloors());
	radarFloors->setNodeMask(CAMERA_MASK_RADAR);
	floorsGroup->addChild(radarFloors);

    return floorsGroup;
}

osg::ref_ptr<osg::Group> LevelView::constructObstacles(int levelSize)
{
	osg::ref_ptr<osg::Group> obstaclesGroup = new osg::Group();
	obstaclesGroup->setName("obstaclesGroup");

	osg::ref_ptr<osg::Node> obstacles = osgDB::readNodeFile("data/models/simple_level.ive");
	obstacles->setCullingActive(false);

	//osg::ref_ptr<osg::Group> obstacles = constructGroupForBoxes(m_model->getObstacles()); 
	osg::StateSet *obstaclesStateSet =  obstacles->getOrCreateStateSet();
	obstaclesStateSet->ref();
	//obstacles->setStateSet(obstaclesStateSet);
	osg::Uniform* textureMapU = new osg::Uniform("diffuseTexture", 0);
	obstaclesStateSet->addUniform(textureMapU);
	setTexture(obstaclesStateSet, "data/textures/box.tga", 0);
	addShaderAndUniforms(obstacles, shaders::DEFAULT, levelSize, GLOW);
	obstacles->setNodeMask(CAMERA_MASK_MAIN);
	obstaclesGroup->addChild(obstacles);

	osg::ref_ptr<osg::Group> radarObstacles = constructRadarElementsForBoxes(m_model->getObstacles());
	radarObstacles->setNodeMask(CAMERA_MASK_RADAR);
	obstaclesGroup->addChild(radarObstacles);

	return obstaclesGroup;
}



void LevelView::addShaderAndUniforms(osg::ref_ptr<osg::Node>& node, int shaderIndex, int levelSize, int modelID)
{
	osg::StateSet *stateSet = node->getOrCreateStateSet();
	stateSet->ref();

	stateSet->setAttributeAndModes(shaders::m_allShaderPrograms[shaderIndex], osg::StateAttribute::ON);
	stateSet->addUniform(new osg::Uniform("levelSize", levelSize));
	stateSet->addUniform(new osg::Uniform("modelID", modelID));
	if (modelID == GLOW)
		stateSet->addUniform(new osg::Uniform("glowIntensity", 1.f));

}

osg::ref_ptr<osg::Group> LevelView::constructGroupForBoxes(std::vector<BoxModel> &boxes)
{
	osg::ref_ptr<osg::Group> boxGroup = new osg::Group();

	for (int i = 0; i < boxes.size(); ++i)
	{
		btVector3 dimensions = boxes[i].dimensions;
		btVector3 position = boxes[i].center;
		btQuaternion rotation = boxes[i].rotation;

		// create obstacle
		osg::ref_ptr<osg::Box> box
				= new osg::Box(osg::Vec3(0,0,0), dimensions.x(), dimensions.y(), dimensions.z());
		osg::ref_ptr<osg::ShapeDrawable> boxDrawable
				= new osg::ShapeDrawable(box);
		osg::ref_ptr<osg::Geode> boxGeode
				= new osg::Geode();
		boxGeode->addDrawable(boxDrawable);

		// place objects in world space
		osg::Matrixd initialTransform;
		initialTransform.makeRotate(btToOSGQuat(rotation));
		initialTransform *= initialTransform.translate(btToOSGVec3(position));

		osg::ref_ptr<osg::MatrixTransform> matrixTransform = new osg::MatrixTransform(initialTransform);
		matrixTransform->addChild(boxGeode);

		boxGroup->addChild(matrixTransform);
	}

	return boxGroup;
}

osg::ref_ptr<osg::Group> LevelView::constructRadarElementsForBoxes(std::vector<BoxModel> &boxes)
{
	osg::ref_ptr<osg::Group> radarBoxGroup = new osg::Group();

	for (int i = 0; i < boxes.size(); ++i)
	{
		btVector3 dimensions = boxes[i].dimensions;
		btVector3 position = boxes[i].center;
		btQuaternion rotation = boxes[i].rotation;

		int threshold = 50;
		if (dimensions.x() < threshold) dimensions.setX(dimensions.x() + threshold);
		if (dimensions.y() < threshold) dimensions.setY(dimensions.y() + threshold);
		if (dimensions.y() < threshold) dimensions.setZ(dimensions.z() + threshold);

		osg::ref_ptr<osg::Box> box
			= new osg::Box(osg::Vec3(0, 0, 0), dimensions.x(), dimensions.y(), dimensions.z());
		osg::ref_ptr<osg::ShapeDrawable> mark_shape = new osg::ShapeDrawable(box);
		mark_shape->setColor(osg::Vec4f(1, 1, 1, .1));
		osg::ref_ptr<osg::Geode> mark_node = new osg::Geode;
		mark_node->addDrawable(mark_shape.get());
		mark_node->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

		// place objects in world space
		osg::Matrixd initialTransform;
		initialTransform.makeRotate(btToOSGQuat(rotation));
		initialTransform *= initialTransform.translate(btToOSGVec3(position));

		osg::ref_ptr<osg::MatrixTransform> matrixTransformRadar = new osg::MatrixTransform(initialTransform);
		matrixTransformRadar->addChild(mark_node);

		radarBoxGroup->addChild(matrixTransformRadar);
	}

	return radarBoxGroup;
}

void LevelView::setTexture(osg::ref_ptr<osg::StateSet> stateset, std::string filePath, int unit)
{

	osg::Image* image = osgDB::readImageFile(filePath);
	if (!image)
		std::cout << "[TroenGame::levelView]  File \"" << filePath << "\" not found." << std::endl;
	else
	{
		osg::Texture2D* texture = new osg::Texture2D;
		texture->setImage(image);
		texture->setResizeNonPowerOfTwoHint(false);
		texture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::REPEAT);
		texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT);
		stateset->setTextureAttributeAndModes(unit, texture, osg::StateAttribute::ON);

	}
}

void LevelView::addItemBox(osg::Vec3 position)
{

	btVector3 dimensions = btVector3(10, 10, 0.1);

	osg::ref_ptr<osg::Box> box
		= new osg::Box(osg::Vec3(0.0, 0.0, 0.0), dimensions.x(), dimensions.y(), dimensions.z());

	osg::ref_ptr<osg::ShapeDrawable> boxDrawable
		= new osg::ShapeDrawable(box);

	osg::ref_ptr<osg::Geode> boxGeode = new osg::Geode();
	boxGeode->addDrawable(boxDrawable);

	osg::StateSet *obstaclesStateSet = boxGeode->getOrCreateStateSet();
	obstaclesStateSet->ref();
	osg::Uniform* textureMapU = new osg::Uniform("diffuseTexture", 0);
	obstaclesStateSet->addUniform(textureMapU);
	setTexture(obstaclesStateSet, "data/textures/turbostrip.tga", 0);

	obstaclesStateSet->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::DEFAULT], osg::StateAttribute::ON);
	obstaclesStateSet->addUniform(new osg::Uniform("levelSize", m_model->getLevelSize()));
	obstaclesStateSet->addUniform(new osg::Uniform("modelID", DEFAULT));

	osg::Matrixd initialTransform;
	initialTransform = initialTransform.translate(position);

	osg::ref_ptr<osg::MatrixTransform> matrixTransform = new osg::MatrixTransform(initialTransform);
	matrixTransform->addChild(boxGeode);
	matrixTransform->setName("itemGeode");


	m_node->addChild(matrixTransform);
}



osg::ref_ptr<osg::Group>  LevelView::getFloor()
{
	return m_node;
}

