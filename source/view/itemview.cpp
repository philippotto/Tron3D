#include "itemview.h"
// OSG
#include <osg/ShapeDrawable>
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

ItemView::ItemView(osg::Vec3 dimensions, osg::Vec3 position, LevelView* levelView)
{
	AbstractView();

	osg::ref_ptr<osg::Box> box
		= new osg::Box(osg::Vec3(0.0, 0.0, 0.0), dimensions.x(), dimensions.y(), dimensions.z());

	osg::ref_ptr<osg::ShapeDrawable> boxDrawable = new osg::ShapeDrawable(box);

	osg::ref_ptr<osg::Geode> boxGeode = new osg::Geode();
	boxGeode->addDrawable(boxDrawable);

	osg::StateSet *obstaclesStateSet = boxGeode->getOrCreateStateSet();
	obstaclesStateSet->ref();
	osg::Uniform* textureMapU = new osg::Uniform("diffuseTexture", 0);
	obstaclesStateSet->addUniform(textureMapU);
	// setTexture(obstaclesStateSet, "data/textures/turbostrip.tga", 0);

	obstaclesStateSet->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::DEFAULT], osg::StateAttribute::ON);
	obstaclesStateSet->addUniform(new osg::Uniform("levelSize", LEVEL_SIZE));
	obstaclesStateSet->addUniform(new osg::Uniform("modelID", AbstractView::DEFAULT));

	osg::Matrixd initialTransform;
	initialTransform = initialTransform.translate(position);

	m_matrixTransform = new osg::MatrixTransform(initialTransform);
	m_matrixTransform->addChild(boxGeode);

	
	levelView->addItemBox(m_matrixTransform);
}

void ItemView::remove()
{
	m_matrixTransform = NULL;
}