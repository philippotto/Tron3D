#include "bikeview.h"


//osg
#include <osg/Texture2D>
#include <osg/TexEnv>
#include <osg/TexGen>
#include <stdio.h>

// troen
#include "shaders.h"
#include "../input/bikeinputstate.h"

using namespace troen;

BikeView::BikeView()
{
	osg::Matrixd initialTransform;
	osg::Quat rotationQuat(osg::DegreesToRadians(90.0f), osg::Vec3d(0.0, 0.0, 1.0));
	initialTransform.makeRotate(rotationQuat);
	initialTransform.translate(0.0, 0.0, -3.0);
	initialTransform.makeScale(osg::Vec3f(5.0f, 5.0f, 5.0f));
	//initialTransform.scale()

	rootNode = new osg::MatrixTransform(initialTransform);

	
	osg::ref_ptr<osg::Node> MovieCycle_Body = createCyclePart("data/models/cycle/MG_MovieCycle_Body_MI.obj",
		"data/models/cycle/MG_MovieCycle_Body_SPEC.tga",
		"data/models/cycle/MG_MovieCycle_BodyHeadLight_EMSS.tga",
		"data/models/cycle/MG_MovieCycle_Body_NORM.tga");


	osg::ref_ptr<osg::Node> MovieCycle_Player_Body = createCyclePart("data/models/cycle/MG_MovieCycle_PlayerBody_MI.obj",
		"data/models/cycle/MG_Player_Body_SPEC.tga",
		"data/models/cycle/MG_Player_Body_EMSS.tga",
		"data/models/cycle/MG_Player_Body_NORM.tga");

	osg::ref_ptr<osg::Node> MovieCycle_Tire = createCyclePart("data/models/cycle/MG_MovieCycle_Tire_MI.obj",
		"",
		"data/models/cycle/MG_MovieCycle_Tire_EMSS.tga",
		"data/models/cycle/MG_MovieCycle_Tire_NORM.tga");
	
	osg::ref_ptr<osg::Node> MovieCycle_Player_Helmet = createCyclePart("data/models/cycle/MG_MovieCycle_PlayerHelmet_MI.obj",
		"data/models/cycle/MG_Player_Helmet_SPEC.tga",
		"data/models/cycle/MG_Player_Helmet_EMSS.tga",
		"data/models/cycle/MG_Player_Helmet_NORM.tga");


	osg::ref_ptr<osg::Node> MovieCycle_Player_Disc = createCyclePart("data/models/cycle/MG_MovieCycle_PlayerDisc_MI.obj",
		"data/models/cycle/MG_Player_Disc_SPEC.tga",
		"data/models/cycle/MG_Player_Disc_EMSS.tga",
		"data/models/cycle/MG_Player_Disc_NORM.tga");

	osg::ref_ptr<osg::Node> MovieCycle_Glass_MI = createCyclePart("data/models/cycle/MG_MovieCycle_Glass_MI.obj",
		"data/models/cycle/Glass.tga",
		"data/models/cycle/Glass.tga",
		"");

	osg::ref_ptr<osg::Node> MovieCycle_Engine = createCyclePart("data/models/cycle/MG_MovieCycle_Engine_MI.obj",
		"",
		"data/models/cycle/MG_MovieCycle_Engine_EMSS.tga",
		"data/models/cycle/MG_MovieCycle_Engine_NORM.tga");

	osg::ref_ptr<osg::Node> MovieCycle_Player_Baton = createCyclePart("data/models/cycle/MG_MovieCycle_PlayerBaton_MI.obj",
		"data/models/cycle/MG_Player_Baton_SPEC.tga",
		"data/models/cycle/MG_Player_Baton_EMSS.tga",
		"data/models/cycle/MG_Player_Baton_NORM.tga");
	
	MovieCycle_Body->asGroup()->addChild(MovieCycle_Player_Body);
	MovieCycle_Body->asGroup()->addChild(MovieCycle_Tire);
	MovieCycle_Body->asGroup()->addChild(MovieCycle_Player_Helmet);
	MovieCycle_Body->asGroup()->addChild(MovieCycle_Player_Disc);
	MovieCycle_Body->asGroup()->addChild(MovieCycle_Glass_MI);
	MovieCycle_Body->asGroup()->addChild(MovieCycle_Engine);
	MovieCycle_Body->asGroup()->addChild(MovieCycle_Player_Baton);
	rootNode->addChild(MovieCycle_Body);


}


osg::ref_ptr<osg::MatrixTransform> BikeView::get_rootNode()
{
	return rootNode;
}



osg::ref_ptr<osg::Node> BikeView::createCyclePart(std::string objFilePath,std::string specularTexturePath,std::string diffuseTexturePath, std::string normalTexturePath)
{
	enum BIKE_TEXTURES { DIFFUSE, SPECULAR, NORMAL };

	std::cout << "[TroenGame::bikeView] Loading Model \"" << objFilePath << "\"" << std::endl;
	osg::ref_ptr<osg::Node> Node = osgDB::readNodeFile(objFilePath);

	osg::ref_ptr<osg::StateSet> NodeState = Node->getOrCreateStateSet();

	if (specularTexturePath != "")
	{
		NodeState->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::DEFAULT], osg::StateAttribute::ON);
		osg::Uniform* specularMapU = new osg::Uniform("specularTexture", SPECULAR);
		NodeState->addUniform(specularMapU);
		setTexture(NodeState, specularTexturePath, SPECULAR);
	}


	if (diffuseTexturePath != "")
	{
		osg::Uniform* diffuseMapU = new osg::Uniform("diffuseTexture", DIFFUSE);
		NodeState->addUniform(diffuseMapU);
		setTexture(NodeState, diffuseTexturePath, DIFFUSE);
	}

	if (normalTexturePath != ""){
		osg::Uniform* normalMapU = new osg::Uniform("normalTexture", NORMAL);
		NodeState->addUniform(normalMapU);
		setTexture(NodeState, normalTexturePath, NORMAL);

	}


	return Node;
}

void BikeView::setTexture(osg::ref_ptr<osg::StateSet> stateset, std::string filePath, int unit)
{

	osg::Image* image = osgDB::readImageFile(filePath);
	if (!image)

	{
		std::cout << "[TroenGame::bikeView]  File \"" << filePath << "\" not found." << std::endl;
	}

	else
	{
		osg::Texture2D* texture = new osg::Texture2D;
		texture->setImage(image);

	/*	osg::TexEnv* texenv = new osg::TexEnv;
		texenv->setMode(osg::TexEnv::BLEND);
		texenv->setColor(osg::Vec4(0.3f, 0.3f, 0.3f, 0.3f));*/

		stateset->setTextureAttributeAndModes(unit, texture, osg::StateAttribute::ON);
		//stateset->setTextureAttribute(1, texenv);

		//rootNode->setStateSet(stateset);
	}


}
