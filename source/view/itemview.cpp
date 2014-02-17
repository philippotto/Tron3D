#include "itemview.h"
// OSG
#include <osg/ShapeDrawable>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/Texture2D>
// troen
#include "../constants.h"
#include "shaders.h"
#include "../controller/itemcontroller.h"
#include "../view/levelview.h"

#include <osg/Geode>
#include <osg/ShapeDrawable>
#include "../view/shaders.h"
#include "osg/MatrixTransform"


using namespace troen;

ItemView::ItemView(osg::Vec3 dimensions, osg::Vec3 position, LevelView* levelView, ItemController::Type type)
{
	AbstractView();

	m_levelView = levelView;

	osg::ref_ptr<osg::Box> box
		= new osg::Box(osg::Vec3(0.0, 0.0, 0.0), dimensions.x(), dimensions.y(), dimensions.z());

	osg::ref_ptr<osg::ShapeDrawable> boxDrawable = new osg::ShapeDrawable(box);

	osg::ref_ptr<osg::Geode> boxGeode = new osg::Geode();
	boxGeode->addDrawable(boxDrawable);

	osg::StateSet *obstaclesStateSet = boxGeode->getOrCreateStateSet();
	obstaclesStateSet->ref();
	osg::Uniform* textureMapU = new osg::Uniform("diffuseTexture", 0);
	obstaclesStateSet->addUniform(textureMapU);
	if (type == ItemController::TURBOSTRIP)
		setTexture(obstaclesStateSet, "data/textures/turbostrip.tga", 0);
	else
		setTexture(obstaclesStateSet, "data/textures/box_health.tga", 0);

	obstaclesStateSet->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::DEFAULT], osg::StateAttribute::ON);
	obstaclesStateSet->addUniform(new osg::Uniform("levelSize", LEVEL_SIZE));
	obstaclesStateSet->addUniform(new osg::Uniform("modelID", AbstractView::GLOW));
	obstaclesStateSet->addUniform(new osg::Uniform("trueColor", true));

	osg::Matrixd initialTransform;
	initialTransform = initialTransform.translate(position);

	m_matrixTransform = new osg::MatrixTransform(initialTransform);
	m_matrixTransform->addChild(boxGeode);

	levelView->addItemBox(m_matrixTransform);
}

void ItemView::setTexture(osg::ref_ptr<osg::StateSet> stateset, std::string filePath, int unit)
{

	osg::Image* image = osgDB::readImageFile(filePath);
	if (!image)
		std::cout << "[TroenGame::levelView]  File \"" << filePath << "\" not found." << std::endl;
	else
	{
		osg::Texture2D* texture = new osg::Texture2D;
		texture->setImage(image);
		texture->setResizeNonPowerOfTwoHint(false);
		stateset->setTextureAttributeAndModes(unit, texture, osg::StateAttribute::ON);
	}
}

void ItemView::remove()
{
	m_levelView->removeItemBox(m_matrixTransform);
	m_matrixTransform = NULL;
}