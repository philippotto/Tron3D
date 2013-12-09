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
#include <stdio.h>
// troen
#include "shaders.h"
#include "../input/bikeinputstate.h"
#include <osgDB/ReadFile>
#include <osg/PositionAttitudeTransform>

#include "../model/bikemodel.h"

using namespace troen;

BikeView::BikeView(osg::Vec3 color)
{
	m_node = new osg::Group();
	m_playerColor = color;
	pat = new osg::PositionAttitudeTransform();
	
#ifndef _DEBUG
	osg::Matrixd initialTransform;
	osg::Quat rotationQuat(osg::DegreesToRadians(180.0f), osg::Vec3d(0.0, 0.0, 1.0));
	initialTransform.makeRotate(rotationQuat);
	initialTransform *= initialTransform.scale(osg::Vec3f(5.0f, 5.0f, 5.0f));
	initialTransform *= initialTransform.translate(0.0, 0.0, -3.0);
	
	// initialTransform *= initialTransform.rotate(rotationQuat);
	
	//initialTransform.scale()
	osg::MatrixTransform* matrixTransform = new osg::MatrixTransform(initialTransform);
	pat->addChild(matrixTransform);
	
	MovieCycle_Body = createCyclePart("data/models/cycle/MG_MovieCycle_Body_MI.obj",
		"data/models/cycle/MG_MovieCycle_Body_SPEC.tga",
		"data/models/cycle/MG_MovieCycle_BodyHeadLight_EMSS.tga",
		"data/models/cycle/MG_MovieCycle_Body_NORM.tga", DEFAULT);


	osg::ref_ptr<osg::Node> MovieCycle_Player_Body = createCyclePart("data/models/cycle/MG_MovieCycle_PlayerBody_MI.obj",
		"data/models/cycle/MG_Player_Body_SPEC.tga",
		"data/models/cycle/MG_Player_Body_EMSS.tga",
		"data/models/cycle/MG_Player_Body_NORM.tga", GLOW, 0.1);

	// set black body parts of player's back to white/gray
	MovieCycle_Player_Body->getStateSet()->addUniform(new osg::Uniform("diffuseMaterialColor", osg::Vec3(0.5f, 0.5f, 0.5f)));


	osg::ref_ptr<osg::Node> MovieCycle_Tire = createCyclePart("data/models/cycle/MG_MovieCycle_Tire_MI.obj",
		"",
		"data/models/cycle/MG_MovieCycle_Tire_EMSS.tga",
		"data/models/cycle/MG_MovieCycle_Tire_NORM.tga", GLOW, 0.5);

	MovieCycle_Tire->getStateSet()->addUniform(new osg::Uniform("diffuseMaterialColor", osg::Vec3(1.f, 1.f, 1.f)));


	
	osg::ref_ptr<osg::Node> MovieCycle_Player_Helmet = createCyclePart("data/models/cycle/MG_MovieCycle_PlayerHelmet_MI.obj",
		"data/models/cycle/MG_Player_Helmet_SPEC.tga",
		"data/models/cycle/MG_Player_Helmet_EMSS.tga",
		"data/models/cycle/MG_Player_Helmet_NORM.tga", DEFAULT);


	osg::ref_ptr<osg::Node> MovieCycle_Player_Disc = createCyclePart("data/models/cycle/MG_MovieCycle_PlayerDisc_MI.obj",
		"data/models/cycle/MG_Player_Disc_SPEC.tga",
		"data/models/cycle/MG_Player_Disc_EMSS.tga",
		"data/models/cycle/MG_Player_Disc_NORM.tga", GLOW);

	MovieCycle_Player_Disc->getStateSet()->addUniform(new osg::Uniform("diffuseMaterialColor", osg::Vec3(1.f, 1.f, 1.f)));

	osg::ref_ptr<osg::Node> MovieCycle_Glass_MI = createCyclePart("data/models/cycle/MG_MovieCycle_Glass_MI.obj",
		"data/models/cycle/Glass.tga",
		"data/models/cycle/Glass.tga",
		"", DEFAULT);

	osg::ref_ptr<osg::Node> MovieCycle_Engine = createCyclePart("data/models/cycle/MG_MovieCycle_Engine_MI.obj",
		"",
		"data/models/cycle/MG_MovieCycle_Engine_EMSS.tga",
		"data/models/cycle/MG_MovieCycle_Engine_NORM.tga", DEFAULT);

	osg::ref_ptr<osg::Node> MovieCycle_Player_Baton = createCyclePart("data/models/cycle/MG_MovieCycle_PlayerBaton_MI.obj",
		"data/models/cycle/MG_Player_Baton_SPEC.tga",
		"data/models/cycle/MG_Player_Baton_EMSS.tga",
		"data/models/cycle/MG_Player_Baton_NORM.tga", GLOW);
	
	MovieCycle_Body->asGroup()->addChild(MovieCycle_Player_Body);
	MovieCycle_Body->asGroup()->addChild(MovieCycle_Tire);
	MovieCycle_Body->asGroup()->addChild(MovieCycle_Player_Helmet);
	MovieCycle_Body->asGroup()->addChild(MovieCycle_Player_Disc);
	MovieCycle_Body->asGroup()->addChild(MovieCycle_Glass_MI);
	MovieCycle_Body->asGroup()->addChild(MovieCycle_Engine);
	MovieCycle_Body->asGroup()->addChild(MovieCycle_Player_Baton);
	matrixTransform->addChild(MovieCycle_Body);

#endif
#ifdef _DEBUG
	
	pat->addChild(osgDB::readNodeFile("data/models/cessna.osgt"));
#endif
	m_node->addChild(pat);
}

osg::ref_ptr<osg::Node> BikeView::createCyclePart(std::string objFilePath, std::string specularTexturePath,
	std::string diffuseTexturePath, std::string normalTexturePath,
	int modelIndex, float glowIntensity)
{
	enum BIKE_TEXTURES { DIFFUSE, SPECULAR, NORMAL };

	std::cout << "[TroenGame::bikeView] Loading Model \"" << objFilePath << "\"" << std::endl;
	osg::Node* Node = osgDB::readNodeFile(objFilePath);

	//osgDB::writeNodeFile(*Node, std::string("file.osg")); //to look at the scenegraph
	osg::ref_ptr<osg::StateSet> NodeState = Node->getOrCreateStateSet();

	osg::ref_ptr<osg::Geode> singleGeode = dynamic_cast<osg::Geode*>(Node->asGroup()->getChild(0));
	osg::ref_ptr<osg::StateSet> childState = singleGeode->getDrawable(0)->getStateSet();
	osg::StateAttribute* stateAttributeMaterial = childState->getAttribute(osg::StateAttribute::MATERIAL);

	osg::Uniform* modelIndexU = new osg::Uniform("modelID", modelIndex);
	NodeState->addUniform(modelIndexU);

	osg::Uniform* glowIntensityU = new osg::Uniform("glowIntensity", glowIntensity);
	NodeState->addUniform(glowIntensityU);

	osg::Uniform* ColorU;
	if (modelIndex == GLOW) {
		ColorU = new osg::Uniform("playerColor", m_playerColor);
	} else {
		ColorU = new osg::Uniform("playerColor", osg::Vec3(1.f, 1.f, 1.f));
	}
	NodeState->addUniform(ColorU);

	if (stateAttributeMaterial != NULL)
	{
		osg::Material *objMaterial = dynamic_cast<osg::Material*>(stateAttributeMaterial);
		
		osg::Vec4 diffuse = objMaterial->getDiffuse(osg::Material::FRONT_AND_BACK);
		osg::Uniform* diffuseMaterialColorU = new osg::Uniform("diffuseMaterialColor",diffuse );
		NodeState->addUniform(diffuseMaterialColorU);

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

	NodeState->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::DEFAULT], osg::StateAttribute::ON);

	if (specularTexturePath != "")
	{
		
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
		std::cout << "[TroenGame::bikeView]  File \"" << filePath << "\" not found." << std::endl;
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

void BikeView::update()
{
	MovieCycle_Body->getStateSet()->addUniform(new osg::Uniform("transform", pat->asMatrixTransform()->getMatrix()));
}
//
//class UpdateCameraAndTexGenCallback : public osg::NodeCallback
//{
//public:
//
//	typedef std::vector< osg::ref_ptr<osg::Camera> >  CameraList;
//
//	UpdateCameraAndTexGenCallback(osg::NodePath& reflectorNodePath, CameraList& Cameras) :
//		_reflectorNodePath(reflectorNodePath),
//		_Cameras(Cameras)
//	{
//	}
//
//	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
//	{
//		// first update subgraph to make sure objects are all moved into position
//		traverse(node, nv);
//
//		// compute the position of the center of the reflector subgraph
//		osg::Matrixd worldToLocal = osg::computeWorldToLocal(_reflectorNodePath);
//		osg::BoundingSphere bs = _reflectorNodePath.back()->getBound();
//		osg::Vec3 position = bs.center();
//
//		typedef std::pair<osg::Vec3, osg::Vec3> ImageData;
//		const ImageData id[] =
//		{
//			ImageData(osg::Vec3(1, 0, 0), osg::Vec3(0, -1, 0)), // +X
//			ImageData(osg::Vec3(-1, 0, 0), osg::Vec3(0, -1, 0)), // -X
//			ImageData(osg::Vec3(0, 1, 0), osg::Vec3(0, 0, 1)), // +Y
//			ImageData(osg::Vec3(0, -1, 0), osg::Vec3(0, 0, -1)), // -Y
//			ImageData(osg::Vec3(0, 0, 1), osg::Vec3(0, -1, 0)), // +Z
//			ImageData(osg::Vec3(0, 0, -1), osg::Vec3(0, -1, 0))  // -Z
//		};
//
//		for (unsigned int i = 0;
//			i<6 && i<_Cameras.size();
//			++i)
//		{
//			osg::Matrix localOffset;
//			localOffset.makeLookAt(position, position + id[i].first, id[i].second);
//
//			osg::Matrix viewMatrix = worldToLocal*localOffset;
//
//			_Cameras[i]->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
//			_Cameras[i]->setProjectionMatrixAsFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 10000.0);
//			_Cameras[i]->setViewMatrix(viewMatrix);
//		}
//	}
//
//protected:
//
//	virtual ~UpdateCameraAndTexGenCallback() {}
//
//	osg::NodePath               _reflectorNodePath;
//	CameraList                  _Cameras;
//};
//
//class TexMatCullCallback : public osg::NodeCallback
//{
//public:
//
//	TexMatCullCallback(osg::TexMat* texmat) :
//		_texmat(texmat)
//	{
//	}
//
//	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
//	{
//		// first update subgraph to make sure objects are all moved into position
//		traverse(node, nv);
//
//		osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
//		if (cv)
//		{
//			osg::Quat quat = cv->getModelViewMatrix()->getRotate();
//			_texmat->setMatrix(osg::Matrix::rotate(quat.inverse()));
//		}
//	}
//
//protected:
//
//	osg::ref_ptr<osg::TexMat>    _texmat;
//};
//
//
//osg::Group* createShadowedScene(osg::Node* reflectedSubgraph, osg::NodePath reflectorNodePath, unsigned int unit, const osg::Vec4& clearColor, unsigned tex_width, unsigned tex_height, osg::Camera::RenderTargetImplementation renderImplementation)
//{
//
//	osg::Group* group = new osg::Group;
//
//	osg::TextureCubeMap* texture = new osg::TextureCubeMap;
//	texture->setTextureSize(tex_width, tex_height);
//
//	texture->setInternalFormat(GL_RGB);
//	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
//	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
//	texture->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);
//	texture->setFilter(osg::TextureCubeMap::MIN_FILTER, osg::TextureCubeMap::LINEAR);
//	texture->setFilter(osg::TextureCubeMap::MAG_FILTER, osg::TextureCubeMap::LINEAR);
//
//	// set up the render to texture cameras.
//	UpdateCameraAndTexGenCallback::CameraList Cameras;
//	for (unsigned int i = 0; i<6; ++i)
//	{
//		// create the camera
//		osg::Camera* camera = new osg::Camera;
//
//		camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		camera->setClearColor(clearColor);
//
//		// set viewport
//		camera->setViewport(0, 0, tex_width, tex_height);
//
//		// set the camera to render before the main camera.
//		camera->setRenderOrder(osg::Camera::PRE_RENDER);
//
//		// tell the camera to use OpenGL frame buffer object where supported.
//		camera->setRenderTargetImplementation(renderImplementation);
//
//		// attach the texture and use it as the color buffer.
//		camera->attach(osg::Camera::COLOR_BUFFER, texture, 0, i);
//
//		// add subgraph to render
//		camera->addChild(reflectedSubgraph);
//
//		group->addChild(camera);
//
//		Cameras.push_back(camera);
//	}
//
//	// create the texgen node to project the tex coords onto the subgraph
//	osg::TexGenNode* texgenNode = new osg::TexGenNode;
//	texgenNode->getTexGen()->setMode(osg::TexGen::REFLECTION_MAP);
//	texgenNode->setTextureUnit(unit);
//	group->addChild(texgenNode);
//
//	// set the reflected subgraph so that it uses the texture and tex gen settings.    
//	{
//		osg::Node* reflectorNode = reflectorNodePath.front();
//		group->addChild(reflectorNode);
//
//		osg::StateSet* stateset = reflectorNode->getOrCreateStateSet();
//		stateset->setTextureAttributeAndModes(unit, texture, osg::StateAttribute::ON);
//		stateset->setTextureMode(unit, GL_TEXTURE_GEN_S, osg::StateAttribute::ON);
//		stateset->setTextureMode(unit, GL_TEXTURE_GEN_T, osg::StateAttribute::ON);
//		stateset->setTextureMode(unit, GL_TEXTURE_GEN_R, osg::StateAttribute::ON);
//		stateset->setTextureMode(unit, GL_TEXTURE_GEN_Q, osg::StateAttribute::ON);
//
//		osg::TexMat* texmat = new osg::TexMat;
//		stateset->setTextureAttributeAndModes(unit, texmat, osg::StateAttribute::ON);
//
//		reflectorNode->setCullCallback(new TexMatCullCallback(texmat));
//	}
//
//	// add the reflector scene to draw just as normal
//	group->addChild(reflectedSubgraph);
//
//	// set an update callback to keep moving the camera and tex gen in the right direction.
//	group->setUpdateCallback(new UpdateCameraAndTexGenCallback(reflectorNodePath, Cameras));
//
//	return group;
//}
