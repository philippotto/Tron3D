#include "ragdollview.h"
// OSG
#include <osg/ShapeDrawable>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/Texture2D>
// troen
#include "../constants.h"
#include "shaders.h"
#include "../view/levelview.h"

#include <osg/Geode>
#include <osg/ShapeDrawable>
#include "../view/shaders.h"
#include "osg/MatrixTransform"


using namespace troen;

RagdollView::RagdollView(osg::Vec3 dimensions, osg::Vec3 position)
{
	AbstractView();

	osg::ref_ptr<osg::Box> box
		= new osg::Box(osg::Vec3(0.0, 0.0, 0.0), dimensions.x(), dimensions.y(), dimensions.z());

	osg::ref_ptr<osg::ShapeDrawable> boxDrawable = new osg::ShapeDrawable(box);

	osg::ref_ptr<osg::Geode> boxGeode = new osg::Geode();
	boxGeode->addDrawable(boxDrawable);

	osg::StateSet *bodyStateSet = boxGeode->getOrCreateStateSet();
	bodyStateSet->ref();
	osg::Uniform* textureMapU = new osg::Uniform("diffuseTexture", 0);
	bodyStateSet->addUniform(textureMapU);
	setTexture(bodyStateSet, "data/textures/white.tga", 0);

	bodyStateSet->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::DEFAULT], osg::StateAttribute::ON);
	bodyStateSet->addUniform(new osg::Uniform("levelSize", LEVEL_SIZE));
	bodyStateSet->addUniform(new osg::Uniform("modelID", AbstractView::GLOW));
	bodyStateSet->addUniform(new osg::Uniform("trueColor", 1.f));

	osg::Matrixd initialTransform;
	initialTransform = initialTransform.translate(position);

	m_matrixTransform = new osg::MatrixTransform(initialTransform);
	m_matrixTransform->addChild(boxGeode);

	m_node->addChild(m_matrixTransform);

}

void RagdollView::setTexture(osg::ref_ptr<osg::StateSet> stateset, std::string filePath, int unit)
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
