#include "bikeview.h"
//osg
#include <osg/Texture2D>
#include <osg/TexEnv>
#include <osg/TexGen>
#include <osg/TextureCubeMap>
#include <osg/TexMat>
#include <osg/Material>
#include <osg/Geode>
#include <osgDB/WriteFile>
#include <osg/CullFace>
#include <osg/TexGenNode>
#include <osgUtil/CullVisitor>
#include <osg/ShapeDrawable>
#include <stdio.h>
#include <osgDB/ReadFile>
#include <osg/PositionAttitudeTransform>
// troen
#include "../constants.h"
#include "shaders.h"
#include "../input/bikeinputstate.h"
#include "../model/bikemodel.h"
#include "playermarker.h"
#include "../resourcepool.h"

using namespace troen;

BikeView::BikeView(osg::Vec3 color, ResourcePool *resourcePool)
{
	AbstractView();
	m_resourcePool = resourcePool;
	m_node = new osg::Group();
	m_playerColor = color;
	pat = new osg::PositionAttitudeTransform();

	osg::Vec4 color4 = osg::Vec4(color, 1.0);
	osg::ref_ptr<osg::Material> material = new osg::Material;
	material->setColorMode(osg::Material::AMBIENT);
	material->setAmbient(osg::Material::FRONT_AND_BACK,
		osg::Vec4(0.8f, 0.8f, 0.8f, 1.0f));
	material->setDiffuse(osg::Material::FRONT_AND_BACK,
		color4*0.8f);
	material->setSpecular(osg::Material::FRONT_AND_BACK, color4);
	material->setShininess(osg::Material::FRONT_AND_BACK, 1.0f);

	osg::Matrixd initialTransform;
	osg::Quat rotationQuat(osg::DegreesToRadians(180.0f), osg::Z_AXIS);
	initialTransform.makeRotate(rotationQuat);

#ifndef true
	initialTransform *= initialTransform.scale(BIKE_VIEW_SCALE_FACTORS);
	initialTransform *= initialTransform.translate(BIKE_VIEW_TRANSLATE_VALUES);

	osg::MatrixTransform* matrixTransform = new osg::MatrixTransform(initialTransform);
	matrixTransform->setNodeMask(CAMERA_MASK_MAIN);

	pat->addChild(matrixTransform);


	MovieCycle_Body = createCyclePart(ResourcePool::MG_MovieCycle_Body_MI,
		ResourcePool::MG_MovieCycle_Body_SPEC,
		ResourcePool::MG_MovieCycle_BodyHeadLight_EMSS,
		ResourcePool::MG_MovieCycle_Body_NORM, DEFAULT);

	osg::ref_ptr<osg::Node> MovieCycle_Player_Body = createCyclePart(ResourcePool::MG_MovieCycle_PlayerBody_MI,
		ResourcePool::MG_Player_Body_SPEC,
		ResourcePool::MG_Player_Body_EMSS,
		ResourcePool::MG_Player_Body_NORM, GLOW, 0.1f);

	osg::ref_ptr<osg::Node> MovieCycle_Tire = createCyclePart(ResourcePool::MG_MovieCycle_Tire_MI,
		ResourcePool::MG_MovieCycle_Tire_DIFF,
		ResourcePool::MG_MovieCycle_Tire_EMSS,
		ResourcePool::MG_MovieCycle_Tire_NORM, GLOW, 0.5f);

	osg::ref_ptr<osg::Node> MovieCycle_Player_Helmet = createCyclePart(ResourcePool::MG_MovieCycle_PlayerHelmet_MI,
		ResourcePool::MG_Player_Helmet_SPEC,
		ResourcePool::MG_Player_Helmet_EMSS,
		ResourcePool::MG_Player_Helmet_NORM, GLOW, 0.3f);

	osg::ref_ptr<osg::Node> MovieCycle_Player_Disc = createCyclePart(ResourcePool::MG_MovieCycle_PlayerDisc_MI,
		ResourcePool::MG_Player_Disc_SPEC,
		ResourcePool::MG_Player_Disc_EMSS,
		ResourcePool::MG_Player_Disc_NORM, GLOW);

	osg::ref_ptr<osg::Node> MovieCycle_Glass_MI = createCyclePart(ResourcePool::MG_MovieCycle_Glass_MI,
		ResourcePool::Glass,
		ResourcePool::Glass,
		ResourcePool::None,
		DEFAULT);

	osg::ref_ptr<osg::Node> MovieCycle_Engine = createCyclePart(ResourcePool::MG_MovieCycle_Engine_MI,
		ResourcePool::None,
		ResourcePool::MG_MovieCycle_Engine_EMSS,
		ResourcePool::MG_MovieCycle_Engine_NORM, DEFAULT);

	osg::ref_ptr<osg::Node> MovieCycle_Player_Baton = createCyclePart(ResourcePool::MG_MovieCycle_PlayerBaton_MI,
		ResourcePool::MG_Player_Baton_SPEC,
		ResourcePool::MG_Player_Baton_EMSS,
		ResourcePool::MG_Player_Baton_NORM, GLOW);

	MovieCycle_Body->asGroup()->addChild(MovieCycle_Player_Body);
	MovieCycle_Body->asGroup()->addChild(MovieCycle_Tire);
	MovieCycle_Body->asGroup()->addChild(MovieCycle_Player_Helmet);
	MovieCycle_Body->asGroup()->addChild(MovieCycle_Player_Disc);
	MovieCycle_Body->asGroup()->addChild(MovieCycle_Glass_MI);
	MovieCycle_Body->asGroup()->addChild(MovieCycle_Engine);
	MovieCycle_Body->asGroup()->addChild(MovieCycle_Player_Baton);
	matrixTransform->addChild(MovieCycle_Body);

#endif
#ifdef false
	osg::MatrixTransform* matrixTransform = new osg::MatrixTransform(initialTransform);
	matrixTransform->setNodeMask(CAMERA_MASK_MAIN);

	osg::ref_ptr<osg::ShapeDrawable> debugShape = new osg::ShapeDrawable;
	debugShape->setShape(new osg::Box(osg::Vec3(), 2 ,4, 2));
	debugShape->setColor(osg::Vec4f(1,1,1,1));
	osg::ref_ptr<osg::Geode> debugNode = new osg::Geode;
	debugNode->addDrawable(debugShape.get());

	matrixTransform->addChild(debugNode);

	pat->addChild(matrixTransform);
#endif

	// create box for radar
	osg::ref_ptr<osg::ShapeDrawable> mark_shape = new osg::ShapeDrawable;
	mark_shape->setShape(new osg::Cone(osg::Vec3(), 120, 300));
	mark_shape->setColor(color4);
	osg::ref_ptr<osg::Geode> mark_node = new osg::Geode;
	mark_node->addDrawable(mark_shape.get());

	osg::Matrixd radarMatrix;
	osg::Quat radarMarkRotationQuat(osg::DegreesToRadians(90.0f), osg::X_AXIS);
	radarMatrix.makeRotate(radarMarkRotationQuat);

	osg::MatrixTransform* radarMatrixTransform = new osg::MatrixTransform(radarMatrix);
	radarMatrixTransform->addChild(mark_node);
	radarMatrixTransform->setNodeMask(CAMERA_MASK_RADAR);

	pat->addChild(radarMatrixTransform);
	pat->addChild(PlayerMarker(color).getNode());
	m_node->addChild(pat);
}

osg::ref_ptr<osg::Node> BikeView::createCyclePart(ResourcePool::ModelResource objName, ResourcePool::TextureResource specularTexturePath,
	ResourcePool::TextureResource diffuseTexturePath, ResourcePool::TextureResource normalTexturePath,
	int modelIndex, float glowIntensity)
{
	enum BIKE_TEXTURES { DIFFUSE, SPECULAR, NORMAL };

	osg::Node* Node = m_resourcePool->getNode(objName);

	//osgDB::writeNodeFile(*Node, std::string("file.osg")); //to look at the scenegraph
	osg::ref_ptr<osg::StateSet> NodeState = Node->getOrCreateStateSet();

	osg::ref_ptr<osg::Geode> singleGeode = dynamic_cast<osg::Geode*>(Node->asGroup()->getChild(0));
	osg::ref_ptr<osg::StateSet> childState = singleGeode->getDrawable(0)->getStateSet();
	osg::StateAttribute* stateAttributeMaterial = childState->getAttribute(osg::StateAttribute::MATERIAL);

	osg::Uniform* modelIndexU = new osg::Uniform("modelID", modelIndex);
	NodeState->addUniform(modelIndexU);

	osg::Uniform* glowIntensityU = new osg::Uniform("glowIntensity", glowIntensity);
	NodeState->addUniform(glowIntensityU);

	if (stateAttributeMaterial != nullptr)
	{
		osg::Material *objMaterial = dynamic_cast<osg::Material*>(stateAttributeMaterial);

		if (modelIndex != GLOW) {
			// if modelIndex == GLOW we will set it later (this avoids some ugly warnings from osg)
			osg::Vec4 diffuse = objMaterial->getDiffuse(osg::Material::FRONT_AND_BACK);
			osg::Uniform* diffuseMaterialColorU = new osg::Uniform("diffuseMaterialColor", diffuse);
			NodeState->addUniform(diffuseMaterialColorU);
		}

		osg::Vec4 ambient = objMaterial->getAmbient(osg::Material::FRONT_AND_BACK);
		osg::Uniform* ambientMaterialColorU = new osg::Uniform("ambientMaterialColor", ambient);
		NodeState->addUniform(ambientMaterialColorU);

		osg::Vec4 specular = objMaterial->getSpecular(osg::Material::FRONT_AND_BACK);
		osg::Uniform* specularMaterialColorU = new osg::Uniform("specularMaterialColor", specular);
		NodeState->addUniform(specularMaterialColorU);

		float shininess = objMaterial->getShininess(osg::Material::FRONT_AND_BACK);
		osg::Uniform* shininessU = new osg::Uniform("shininess", shininess);
		NodeState->addUniform(shininessU);
	}

	osg::Uniform* ColorU;
	if (modelIndex == GLOW) {
		ColorU = new osg::Uniform("playerColor", m_playerColor);
		// set parts to white/gray so that we can color it
		NodeState->addUniform(new osg::Uniform("diffuseMaterialColor", osg::Vec4(0.5f, 0.5f, 0.5f, 1.f)));
	}
	else {
		ColorU = new osg::Uniform("playerColor", osg::Vec3(1.f, 1.f, 1.f));
	}
	NodeState->addUniform(ColorU);

	if (specularTexturePath != ResourcePool::None)
	{
		osg::Uniform* specularMapU = new osg::Uniform("specularTexture", SPECULAR);
		NodeState->addUniform(specularMapU);
		setTexture(NodeState, specularTexturePath, SPECULAR);
	}

	if (diffuseTexturePath != ResourcePool::None)
	{
		osg::Uniform* diffuseMapU = new osg::Uniform("diffuseTexture", DIFFUSE);
		NodeState->addUniform(diffuseMapU);
		setTexture(NodeState, diffuseTexturePath, DIFFUSE);
	}

	if (normalTexturePath != ResourcePool::None){
		osg::Uniform* normalMapU = new osg::Uniform("normalTexture", NORMAL);
		NodeState->addUniform(normalMapU);
		setTexture(NodeState, normalTexturePath, NORMAL);
	}

	NodeState->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::BIKE], osg::StateAttribute::ON);

	return Node;
}

void BikeView::setTexture(osg::ref_ptr<osg::StateSet> stateset, ResourcePool::TextureResource textureName, int unit)
{

	//osg::Image* image = osgDB::readImageFile(filePath);
	osg::Image* image = m_resourcePool->getImage(textureName);

	osg::Texture2D* texture = new osg::Texture2D;
	texture->setImage(image);
	texture->setResizeNonPowerOfTwoHint(false);

	stateset->setTextureAttributeAndModes(unit, texture, osg::StateAttribute::ON);


}

void BikeView::update()
{
	MovieCycle_Body->getStateSet()->addUniform(new osg::Uniform("transform", pat->asMatrixTransform()->getMatrix()));
}


void BikeView::createPlayerMarker(osg::Vec3 color)
{
	//PlayerMarker *marker = new PlayerMarker(color)
	m_playermarkerNode = PlayerMarker(color).getNode();
	pat->addChild(m_playermarkerNode);
}
