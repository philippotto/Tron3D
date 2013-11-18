#include "levelview.h"
// STD
#include <math.h>
#include <memory>
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


#include "../model/levelmodel.h"

using namespace troen;


LevelView::LevelView(std::shared_ptr<LevelModel> model)
{
	m_model = model;
	initialize();
}

void LevelView::initialize()
{

	int levelSize = m_model->getLevelSize();

	m_node = new osg::Group();

	osg::ref_ptr<osg::Group> levelGroup = new osg::Group();
	osg::ref_ptr<osg::Geode> levelGeode = new osg::Geode();

	// wall right
	osg::ref_ptr<osg::Box> wallRight
		= new osg::Box(osg::Vec3(levelSize / 2, 0, 50), 1, levelSize, 100);
	osg::ref_ptr<osg::ShapeDrawable> wallDrawableRight
		= new osg::ShapeDrawable(wallRight);

	// wall left
	osg::ref_ptr<osg::Box> wallLeft
		= new osg::Box(osg::Vec3(-levelSize / 2, 0, 50), 1, levelSize, 100);
	osg::ref_ptr<osg::ShapeDrawable> wallDrawableLeft
		= new osg::ShapeDrawable(wallLeft);


	// wall back
	osg::ref_ptr<osg::Box> wallBack
		= new osg::Box(osg::Vec3(0, -levelSize / 2, 50), levelSize, 1, 100);
	osg::ref_ptr<osg::ShapeDrawable> wallDrawableBack
		= new osg::ShapeDrawable(wallBack);

	// wall front
	osg::ref_ptr<osg::Box> wallFront
		= new osg::Box(osg::Vec3(0, levelSize / 2, 50), levelSize, 1, 100);
	osg::ref_ptr<osg::ShapeDrawable> wallDrawableFront
		= new osg::ShapeDrawable(wallFront);

	levelGeode->addDrawable(wallDrawableLeft);
	levelGeode->addDrawable(wallDrawableRight);
	levelGeode->addDrawable(wallDrawableFront);
	levelGeode->addDrawable(wallDrawableBack);

	m_node->addChild(constructGround());
	m_node->addChild(levelGeode);
}

osg::ref_ptr<osg::Geode>  LevelView::constructGround()
{
	int levelSize = m_model->getLevelSize();

	osg::Vec3Array *vertexArray = new osg::Vec3Array();

	vertexArray->push_back(osg::Vec3(-levelSize / 2, -levelSize / 2, 0));
	vertexArray->push_back(osg::Vec3(levelSize / 2, -levelSize / 2, 0));
	vertexArray->push_back(osg::Vec3(levelSize / 2, levelSize / 2, 0));
	vertexArray->push_back(osg::Vec3(-levelSize / 2, levelSize / 2, 0));

	// face array
	osg::DrawElementsUInt *faceArray = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);

	faceArray->push_back(0); // face 1
	faceArray->push_back(1);
	faceArray->push_back(2);
	faceArray->push_back(2); // face 2
	faceArray->push_back(3);
	faceArray->push_back(0);

	// normal array
	osg::Vec3Array *normalArray = new osg::Vec3Array();
	normalArray->push_back(osg::Vec3(0, 0, 1));

	// texture coordinates
	osg::Vec2Array *texCoords = new osg::Vec2Array();
	texCoords->push_back(osg::Vec2(0.0, 0.0));
	texCoords->push_back(osg::Vec2(18.0, 0.0));
	texCoords->push_back(osg::Vec2(18.0, 18.0));
	texCoords->push_back(osg::Vec2(0.0, 18.0));

	osg::Geometry *geometry = new osg::Geometry();
	geometry->setVertexArray(vertexArray);
	geometry->setNormalArray(normalArray);
	geometry->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
	geometry->setTexCoordArray(0, texCoords);
	geometry->addPrimitiveSet(faceArray);

	osg::ref_ptr<osg::Geode> plane = new osg::Geode();
	plane->addDrawable(geometry);

	// create a simple material
	osg::Material *material = new osg::Material();
	material->setEmission(osg::Material::FRONT, osg::Vec4(0.8, 0.8, 0.8, 1.0));

	// create a texture
	// load image for texture
	osg::Image *image = osgDB::readImageFile("data/models/Images/grid.tga");
	if (!image) {
		std::cout << "[LevelView::constructGround] Couldn't load texture." << std::endl;
		return NULL;
	}
	osg::Texture2D *texture = new osg::Texture2D;
	texture->setDataVariance(osg::Object::DYNAMIC);
	texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
	texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
	texture->setImage(image);

	// assign the material and texture to the sphere
	osg::StateSet *groundStateSet = plane->getOrCreateStateSet();
	groundStateSet->ref();
	groundStateSet->setAttribute(material);
	groundStateSet->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);

	return  plane;
}