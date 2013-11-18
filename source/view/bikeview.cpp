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

	//loadShaderSource(m_vShader, "data/shaders/default.vert");
	//loadShaderSource(m_fShader, "data/shaders/default.frag");

	//m_program->addShader(m_vShader);
	//m_program->addShader(m_fShader);

	

	osg::Node *body_MI_Node = osgDB::readNodeFile("data/models/cycle/MG_MovieCycle_Body_MI.obj");
	
	osg::ref_ptr<osg::StateSet> body_MI_NodeState = body_MI_Node->getOrCreateStateSet();

	body_MI_NodeState->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::DEFAULT], osg::StateAttribute::ON);
	osg::Uniform* specularMapU = new osg::Uniform("specularTexture", 1);
	body_MI_NodeState->addUniform(specularMapU);
	
	setTexture(body_MI_NodeState, "data/models/cycle/MG_MovieCycle_Body_SPEC.tga");




	rootNode->addChild(body_MI_Node);
}


osg::ref_ptr<osg::MatrixTransform> BikeView::get_rootNode()
{
	return rootNode;
}


void BikeView::setTexture(osg::ref_ptr<osg::StateSet> stateset, std::string filePath)
{

	osg::Image* image = osgDB::readImageFile(filePath);
	if (!image)

	{
		std::cout << "[TroenGame::bikeView]  File \"" << "data/models/MG_MovieCycle_Body_SPEC.tga" << "\" not found." << std::endl;
	}

	else
	{
		osg::Texture2D* texture = new osg::Texture2D;
		texture->setImage(image);

		//osg::TexGen* texgen = new osg::TexGen;
		//texgen->setMode(osg::TexGen::SPHERE_MAP);

		osg::TexEnv* texenv = new osg::TexEnv;
		texenv->setMode(osg::TexEnv::BLEND);
		texenv->setColor(osg::Vec4(0.3f, 0.3f, 0.3f, 0.3f));

		//osg::StateSet* stateset = new osg::StateSet;
		stateset->setTextureAttributeAndModes(1, texture, osg::StateAttribute::ON);
		//stateset->setTextureAttributeAndModes(1, texgen, osg::StateAttribute::ON);
		stateset->setTextureAttribute(1, texenv);

		//rootNode->setStateSet(stateset);
	}


}
