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
	osg::ref_ptr<osg::Geode> outerWallsGeode = new osg::Geode();

	// wall right
	osg::ref_ptr<osg::Box> wallRight
		= new osg::Box(osg::Vec3(levelSize / 2, 0, 10), 1, levelSize, 20);
	osg::ref_ptr<osg::ShapeDrawable> wallDrawableRight
		= new osg::ShapeDrawable(wallRight);

	// wall left
	osg::ref_ptr<osg::Box> wallLeft
		= new osg::Box(osg::Vec3(-levelSize / 2, 0, 10),1, levelSize, 20);
	osg::ref_ptr<osg::ShapeDrawable> wallDrawableLeft
		= new osg::ShapeDrawable(wallLeft);


	// wall back
	osg::ref_ptr<osg::Box> wallBack
		= new osg::Box(osg::Vec3(0, -levelSize / 2, 10), levelSize, 1, 20);
	osg::ref_ptr<osg::ShapeDrawable> wallDrawableBack
		= new osg::ShapeDrawable(wallBack);

	// wall front
	osg::ref_ptr<osg::Box> wallFront
		= new osg::Box(osg::Vec3(0, levelSize / 2, 10), levelSize, 1, 20);
	osg::ref_ptr<osg::ShapeDrawable> wallDrawableFront
		= new osg::ShapeDrawable(wallFront);

	outerWallsGeode->addDrawable(wallDrawableLeft);
	outerWallsGeode->addDrawable(wallDrawableRight);
	outerWallsGeode->addDrawable(wallDrawableFront);
	outerWallsGeode->addDrawable(wallDrawableBack);

	osg::StateSet *wallStateSet = outerWallsGeode->getOrCreateStateSet();
	wallStateSet->ref();


	wallStateSet->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::OUTER_WALL], osg::StateAttribute::ON);
	osg::Uniform* levelSizeUniform = new osg::Uniform("levelSize", levelSize);
	wallStateSet->addUniform(levelSizeUniform);

	osg::Uniform* modelIDU = new osg::Uniform("modelID", DEFAULT);
	wallStateSet->addUniform(modelIDU);


	m_node->addChild(constructBox(osg::Vec3(10.0,0.0,50.0),osg::Vec3(50.0,50.0,100.0)));

	m_node->addChild(outerWallsGeode);
	m_node->addChild(constructGround());
	//m_node->addChild(constructObstacles());
	m_node->addChild(autoConstructObstacles());
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
	// osg::Material *material = new osg::Material();
	// material->setEmission(osg::Material::FRONT, osg::Vec4(0.8, 0.8, 0.8, 1.0));

	// create a texture
	// load image for texture
	//osg::Image *image = osgDB::readImageFile("data/models/Images/grid.tga");
	/*if (!image) {
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
	*/
	osg::StateSet *groundStateSet = plane->getOrCreateStateSet();
	groundStateSet->ref();


	//groundStateSet->setAttribute(material);
	//groundStateSet->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
	
	
	groundStateSet->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::GRID], osg::StateAttribute::ON);
	osg::Uniform* levelSizeUniform = new osg::Uniform("levelSize", levelSize);
	groundStateSet->addUniform(levelSizeUniform);


	osg::Uniform* modelIDU = new osg::Uniform("modelID", DEFAULT);
	groundStateSet->addUniform(modelIDU);
	//setTexture(groundStateSet, specularTexturePath, SPECULAR);

	return  plane;
}




osg::ref_ptr<osg::Geode> LevelView::constructBox(osg::Vec3 position,osg::Vec3 scale)
{
	int levelSize = m_model->getLevelSize();
	osg::Geometry* boxGeometry = new osg::Geometry;

	// Vertices
	osg::Vec3Array* vertices = new osg::Vec3Array;
	//somehow postscale doesnt work..
	vertices->push_back(osg::Vec3(-.5*scale.x(), -.5*scale.y(), -.5*scale.z()) + position); // 0
	vertices->push_back(osg::Vec3(.5*scale.x(), -.5*scale.y(), -.5*scale.z()) + position); // 3
	vertices->push_back(osg::Vec3(.5*scale.x(), -.5*scale.y(), .5*scale.z()) + position); // 5
	vertices->push_back(osg::Vec3(-.5*scale.x(), -.5*scale.y(), .5*scale.z()) + position); // 1

	vertices->push_back(osg::Vec3(-.5*scale.x(), -.5*scale.y(), .5*scale.z()) + position); // 1
	vertices->push_back(osg::Vec3(.5*scale.x(), -.5*scale.y(), .5*scale.z()) + position); // 5
	vertices->push_back(osg::Vec3(.5*scale.x(), .5*scale.y(), .5*scale.z()) + position); // 7
	vertices->push_back(osg::Vec3(-.5*scale.x(), .5*scale.y(), .5*scale.z()) + position); // 4

	vertices->push_back(osg::Vec3(-.5*scale.x(), -.5*scale.y(), -.5*scale.z()) + position); // 0
	vertices->push_back(osg::Vec3(-.5*scale.x(), -.5*scale.y(), .5*scale.z()) + position); // 1
	vertices->push_back(osg::Vec3(-.5*scale.x(), .5*scale.y(), .5*scale.z()) + position); // 4
	vertices->push_back(osg::Vec3(-.5*scale.x(), .5*scale.y(), -.5*scale.z()) + position); // 2

	vertices->push_back(osg::Vec3(.5*scale.x(), -.5*scale.y(), -.5*scale.z()) + position); // 3
	vertices->push_back(osg::Vec3(.5*scale.x(), .5*scale.y(), -.5*scale.z()) + position); // 6
	vertices->push_back(osg::Vec3(.5*scale.x(), .5*scale.y(), .5*scale.z()) + position); // 7
	vertices->push_back(osg::Vec3(.5*scale.x(), -.5*scale.y(), .5*scale.z()) + position); // 5

	vertices->push_back(osg::Vec3(.5*scale.x(), .5*scale.y(), -.5*scale.z()) + position); // 6
	vertices->push_back(osg::Vec3(-.5*scale.x(), .5*scale.y(), -.5*scale.z()) + position); // 2
	vertices->push_back(osg::Vec3(-.5*scale.x(), .5*scale.y(), .5*scale.z()) + position); // 4
	vertices->push_back(osg::Vec3(.5*scale.x(), .5*scale.y(), .5*scale.z()) + position); // 7

	vertices->push_back(osg::Vec3(.5*scale.x(), -.5*scale.y(), -.5*scale.z()) + position); // 3
	vertices->push_back(osg::Vec3(-.5*scale.x(), -.5*scale.y(), -.5*scale.z()) + position); // 0
	vertices->push_back(osg::Vec3(-.5*scale.x(), .5*scale.y(), -.5*scale.z()) + position); // 2
	vertices->push_back(osg::Vec3(.5*scale.x(), .5*scale.y(), -.5*scale.z()) + position); // 6

	boxGeometry->setVertexArray(vertices);

	// TexCoords
	osg::Vec2Array *texCoords = new osg::Vec2Array();
	for (int i = 0; i < 6; i++){
		texCoords->push_back(osg::Vec2(0.0, 0.0));
		texCoords->push_back(osg::Vec2(1.0, 0.0));
		texCoords->push_back(osg::Vec2(1.0, 1.0));
		texCoords->push_back(osg::Vec2(0.0, 1.0));
	}
	boxGeometry->setTexCoordArray(0, texCoords);

	// Normals
	osg::Vec3Array* normals = new osg::Vec3Array;
	normals->push_back(osg::Vec3(0, -1, 0));
	normals->push_back(osg::Vec3(0, -1, 0));
	normals->push_back(osg::Vec3(0, -1, 0));
	normals->push_back(osg::Vec3(0, -1, 0));

	normals->push_back(osg::Vec3(0, 0, 1));
	normals->push_back(osg::Vec3(0, 0, 1));
	normals->push_back(osg::Vec3(0, 0, 1));
	normals->push_back(osg::Vec3(0, 0, 1));

	normals->push_back(osg::Vec3(-1, 0, 0));
	normals->push_back(osg::Vec3(-1, 0, 0));
	normals->push_back(osg::Vec3(-1, 0, 0));
	normals->push_back(osg::Vec3(-1, 0, 0));

	normals->push_back(osg::Vec3(1, 0, 0));
	normals->push_back(osg::Vec3(1, 0, 0));
	normals->push_back(osg::Vec3(1, 0, 0));
	normals->push_back(osg::Vec3(1, 0, 0));

	normals->push_back(osg::Vec3(0, 1, 0));
	normals->push_back(osg::Vec3(0, 1, 0));
	normals->push_back(osg::Vec3(0, 1, 0));
	normals->push_back(osg::Vec3(0, 1, 0));

	normals->push_back(osg::Vec3(0, 0, -1));
	normals->push_back(osg::Vec3(0, 0, -1));
	normals->push_back(osg::Vec3(0, 0, -1));
	normals->push_back(osg::Vec3(0, 0, -1));
	boxGeometry->setNormalArray(normals);
	boxGeometry->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

	// Faces
	boxGeometry->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0,
		vertices->size()));

	//osg::DrawElementsUInt *faceArray = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);

	//for (int i = 0; i < 6 * 4; i++)
	//	faceArray->push_back(i);
	//boxGeometry->addPrimitiveSet(faceArray);


	osg::ref_ptr<osg::Geode> boxGeode = new osg::Geode();
	boxGeode->addDrawable(boxGeometry);

	osg::StateSet *boxStateSet = boxGeode->getOrCreateStateSet();
	boxStateSet->ref();

	boxStateSet->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::DEFAULT], osg::StateAttribute::ON);

	osg::Uniform* textureMapU = new osg::Uniform("diffuseTexture", levelSize);
	boxStateSet->addUniform(textureMapU);

	setTexture(boxStateSet, "data/textures/troen_box_tex.tga", 0);


	osg::Uniform* modelIDU = new osg::Uniform("modelID", DEFAULT);
	boxStateSet->addUniform(modelIDU);

	return boxGeode;
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
		stateset->setTextureAttributeAndModes(unit, texture, osg::StateAttribute::ON);

	}
}


osg::ref_ptr<osg::Group> LevelView::constructObstacles()
{
	int levelSize = m_model->getLevelSize();
	osg::ref_ptr<osg::Group> obstacleGroup = new osg::Group();

	obstacleGroup->addChild(constructSimpleBox(osg::Vec3(-20, -30, 10), osg::Vec3(5, 5, 20), osg::Quat(0.0, 0.0, 0.0, 1.0)));

	osg::StateSet *obstaclesStateSet = obstacleGroup->getOrCreateStateSet();
	obstaclesStateSet->ref();

	obstaclesStateSet->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::DEFAULT], osg::StateAttribute::ON);

	setTexture(obstaclesStateSet, "data/textures/troen_box_tex.tga", 0);

	osg::Uniform* textureMapU = new osg::Uniform("diffuseTexture", 0);
	obstaclesStateSet->addUniform(textureMapU);

	osg::Uniform* modelIDU = new osg::Uniform("modelID", DEFAULT);
	obstaclesStateSet->addUniform(modelIDU);

	return obstacleGroup;
}

osg::ref_ptr<osg::MatrixTransform> LevelView::constructSimpleBox(osg::Vec3 position, osg::Vec3 extents, osg::Quat rotation)
{
	osg::ref_ptr<osg::Box> box1
		= new osg::Box(osg::Vec3(0.0,0.0,0.0), extents.x(), extents.y(), extents.z());
	osg::ref_ptr<osg::ShapeDrawable> boxDrawable1
		= new osg::ShapeDrawable(box1);

	osg::ref_ptr<osg::Geode> boxGeode = new osg::Geode();
	boxGeode->addDrawable(boxDrawable1);

	osg::Matrixd initialTransform;
	initialTransform.makeRotate(rotation);
	initialTransform *= initialTransform.translate(position);

	osg::ref_ptr<osg::MatrixTransform> matrixTransform = new osg::MatrixTransform(initialTransform);
	matrixTransform->addChild(boxGeode);


	return matrixTransform;

}