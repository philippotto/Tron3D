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

#include <osg/PolygonMode>
// bullet
#include <btBulletDynamicsCommon.h>
// troen
#include "shaders.h"
#include "../constants.h"

using namespace troen;


class CategorizeLevelElements : public osg::NodeVisitor
{
public:
	CategorizeLevelElements()
		: osg::NodeVisitor( // Traverse all children.
		osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
	{
			boxesNode = new osg::Group();
			rampsNode = new osg::Group();
			raisedFloorsNode = new osg::Group();
		}

	// This method gets called for every node in the scene
	//   graph. Check each node to see if its name matches
	//   out target. If so, save the node's address.
	virtual void apply(osg::Node& node)
	{
		if (node.getName().find("ramp") != std::string::npos)
			rampsNode->addChild(&node);
		else if (node.getName().find("floor") != std::string::npos || node.getName().find("raisedFloor") != std::string::npos)
			raisedFloorsNode->addChild(&node);
		else if (node.getName().find("Cube") != std::string::npos) //this check is necessary because toplevelnode is whole obj, which we dont want to add
			boxesNode->addChild(&node);

		// Keep traversing the rest of the scene graph.
		traverse(node);
	}

	osg::ref_ptr<osg::Group> getRampNode() { return rampsNode; }
	osg::ref_ptr<osg::Group> getBoxesNode()  { return boxesNode; }
	osg::ref_ptr<osg::Group> getRaisedFloorsNode()  { return raisedFloorsNode; }

protected:
	std::string _name;
	osg::ref_ptr<osg::Group> boxesNode;
	osg::ref_ptr<osg::Group> rampsNode;
	osg::ref_ptr<osg::Group> raisedFloorsNode;
};


LevelView::LevelView(std::shared_ptr<LevelModel> model, std::string levelName) :
AbstractView()
{
	m_model = model;

	int levelSize = m_model->getLevelSize();

	m_node->addChild(constructObstacles(levelSize, levelName));
	m_node->addChild(constructFloors(levelSize));


	osg::StateSet *stateSet = m_node->getOrCreateStateSet();
	m_bendedUniform = new osg::Uniform("bendingFactor", 0.f);
	stateSet->addUniform(m_bendedUniform);

	m_bendingActiveUniform = new osg::Uniform("bendingActivated",false);
	stateSet->addUniform(m_bendingActiveUniform);

	m_itemGroup = new osg::Group();
	m_itemGroup->setName("itemGroup");
	m_node->addChild(m_itemGroup);
}

void LevelView::reload(std::string levelName)
{
	m_node->removeChildren(0, m_node->getNumChildren());

	int levelSize = m_model->getLevelSize();

	m_node->addChild(constructObstacles(levelSize, levelName));
	m_node->addChild(constructFloors(levelSize));
}

osg::ref_ptr<osg::Group> LevelView::constructFloors(int levelSize)
{
	osg::ref_ptr<osg::Group> floorsGroup = new osg::Group();

	osg::ref_ptr<osg::Node> floors = osgDB::readNodeFile("data/models/floor_highres.ive");
	floors->setNodeMask(CAMERA_MASK_MAIN);

	 //osg::ref_ptr<osg::Group> floors = constructGroupForBoxes(m_model->getFloors());
	floors->setName("floorsNode");

	osg::StateSet *obstaclesStateSet = floors->getOrCreateStateSet();
	osg::Uniform* textureMapU = new osg::Uniform("diffuseTexture", 0);
	obstaclesStateSet->addUniform(textureMapU);
	setTexture(obstaclesStateSet, "data/textures/floor.tga", 0);

	//will be overwritten if reflection is used
	addShaderAndUniforms(static_cast<osg::ref_ptr<osg::Node>>(floors), shaders::GRID_NOREFLECTION, levelSize, GLOW, 1.0);


	floors->setNodeMask(CAMERA_MASK_MAIN);
	floorsGroup->addChild(floors);


	osg::ref_ptr<osg::Group> radarFloors = constructRadarElementsForBoxes(m_model->getFloors());
	radarFloors->setNodeMask(CAMERA_MASK_RADAR);
	floorsGroup->addChild(radarFloors);


  return floorsGroup;
}

osg::ref_ptr<osg::Group> LevelView::constructObstacles(int levelSize, std::string levelName)
{
	osg::ref_ptr<osg::Group> obstaclesGroup = new osg::Group();
	obstaclesGroup->setName("obstaclesGroup");

	osg::ref_ptr<osg::Group> mainGroup = new osg::Group();

	osg::ref_ptr<osg::Node> readObstacles = osgDB::readNodeFile("data/levels/" + levelName + ".ive");
	readObstacles->setCullingActive(false);


	CategorizeLevelElements *findObjects = new CategorizeLevelElements();
	readObstacles->accept(*findObjects);
	osg::ref_ptr<osg::Group> rampsNode = findObjects->getRampNode();
	osg::ref_ptr<osg::Group> boxesNode = findObjects->getBoxesNode();

	osg::ref_ptr<osg::Group> floorsNode = findObjects->getRaisedFloorsNode();


	mainGroup->addChild(rampsNode);
	mainGroup->addChild(floorsNode);
	mainGroup->addChild(boxesNode);



	osg::Uniform* textureMapU = new osg::Uniform("diffuseTexture", 0);

	addShaderAndUniforms(floorsNode, shaders::DEFAULT, levelSize, GLOW, 0.5, 1.0);
	addShaderAndUniforms(boxesNode, shaders::DEFAULT, levelSize, GLOW, 1.0);
	addShaderAndUniforms(rampsNode, shaders::DEFAULT, levelSize, GLOW, 1.0);
	
	setTexture(boxesNode->getOrCreateStateSet(), "data/textures/box.tga", 0);
	setTexture(floorsNode->getOrCreateStateSet(), "data/textures/raisedlevel.tga", 0, true);
	setTexture(rampsNode->getOrCreateStateSet(), "data/textures/ramptexture.tga", 0);
	
	boxesNode->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::OFF);
	floorsNode->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	floorsNode->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON );

	mainGroup->setNodeMask(CAMERA_MASK_MAIN);


	osg::ref_ptr<osg::Group> radarObstacles = constructRadarElementsForBoxes(m_model->getObstacles());
	radarObstacles->setNodeMask(CAMERA_MASK_RADAR);
	obstaclesGroup->addChild(radarObstacles);
	obstaclesGroup->addChild(mainGroup);

	std::cout << "Obstacles radius" << obstaclesGroup->getBound().radius() << std::endl;

	return obstaclesGroup;
}


void LevelView::addShaderAndUniforms(osg::ref_ptr<osg::Node> node, int shaderIndex, int levelSize, int modelID, float alpha, float trueColor /*= 0.0*/)
{
	osg::StateSet *stateSet = node->getOrCreateStateSet();
	stateSet->ref();

	stateSet->setAttributeAndModes(shaders::m_allShaderPrograms[shaderIndex], osg::StateAttribute::ON);
	stateSet->addUniform(new osg::Uniform("levelSize", levelSize));
	stateSet->addUniform(new osg::Uniform("modelID", modelID));
	stateSet->addUniform(new osg::Uniform("trueColor", trueColor));
	stateSet->addUniform(new osg::Uniform("alpha", alpha));
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


		osg::ref_ptr<osg::ShapeDrawable> boxDrawable
			= new osg::ShapeDrawable(new osg::Box(osg::Vec3(0, 0, 0), dimensions.x(), dimensions.y(), dimensions.z()));


		//osg::ref_ptr<osg::Geode> boxGeode
		//	= createCube(osg::Vec3(0, 0, 0), dimensions.x(), dimensions.y(), dimensions.z());// new osg::Geode();
		osg::ref_ptr<osg::Geode> boxGeode = new osg::Geode();
		boxGeode->addDrawable(boxDrawable);
		//boxGeode->getOrCreateStateSet()->setAttribute(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE));

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
		mark_shape->setColor(osg::Vec4f(1, 1, 1, .5));
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

void LevelView::setTexture(osg::ref_ptr<osg::StateSet> stateset, std::string filePath, int unit, bool override)
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
		if (override)
			stateset->setTextureAttributeAndModes(unit, texture, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
		else
			stateset->setTextureAttributeAndModes(unit, texture, osg::StateAttribute::ON);
	}
}

void LevelView::addItemBox(osg::ref_ptr<osg::MatrixTransform>& matrixTransform)
{
	m_itemGroup->addChild(matrixTransform);
}

void troen::LevelView::removeItemBox(osg::ref_ptr<osg::MatrixTransform>& matrixTransform)
{
	m_itemGroup->removeChild(matrixTransform);
}


osg::ref_ptr<osg::Group>  LevelView::getFloor()
{
	return m_node;
}

void LevelView::setBendingFactor(float bendingFactor)
{
	m_bendedUniform->set(bendingFactor);
}
void LevelView::setBendingActive(bool val)
{
	m_bendingActiveUniform->set(val);
}



