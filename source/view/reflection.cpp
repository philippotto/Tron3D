#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <osg/Geode>
#include <osg/ArgumentParser>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/TexEnv>
#include <osg/TexGen>
#include <osg/TexMat>
#include <osg/TextureCubeMap>
#include <osg/Program>
#include <osg/MatrixTransform>
#include <osg/Shader>
#include <osg/io_utils>
#include <osg/Uniform>
#include <osg/Camera>
#include <osg/LightModel>
#include <osg/ClipNode>


//troen
#include "reflection.h"
#include "shaders.h"
#include "skydome.h"
#include "../constants.h"

/*
**
*/
using namespace troen;

static osg::ref_ptr<osg::Uniform> g_cameraEyeU = new osg::Uniform("cameraEye", osg::Vec3(0.0, 0.0,0.0));

class CUpdateCameraCallback : public osg::NodeCallback
{
public:
	osg::ref_ptr <osgViewer::View> m_gameView;
	osg::Matrixd m_reflectionMatrix;
	
	CUpdateCameraCallback(osg::ref_ptr <osgViewer::View> view) : NodeCallback()
	{
		m_gameView = view;
		m_reflectionMatrix = osg::Matrixd::scale(1.0, 1.0, -1.0);
	}

	void operator()(osg::Node *node, osg::NodeVisitor *nv)
	{
		if (nv->getVisitorType() == osg::NodeVisitor::CULL_VISITOR)
		{
			osgUtil::CullVisitor	*cv = static_cast<osgUtil::CullVisitor *>(nv);
			osg::Camera				*camera = static_cast<osg::Camera *>(node->asGroup()->getChild(0));

			//camera->setProjectionMatrix(m_gameView->getCamera()->getProjectionMatrix());
			//camera->setViewMatrix(m_gameView->getCamera()->getViewMatrix());

			//camera->setProjectionMatrix(cv->getCurrentCamera()->getProjectionMatrix());
			//camera->setViewMatrix(cv->getCurrentCamera()->getViewMatrix());

			camera->setViewMatrix(m_gameView->getCamera()->getViewMatrix());
			camera->setProjectionMatrix(m_gameView->getCamera()->getProjectionMatrix());

			//g_cameraEyeU->set(osg::Vec3(0.0, 0.0, 0.0) * m_gameView->getCamera()->getInverseViewMatrix());

			//_reflectionCamera->accept(*_cv);

		}
		this->traverse(node, nv);
	}
};

class FindNamedNode : public osg::NodeVisitor
{
public:
	FindNamedNode(const std::string& name)
		: osg::NodeVisitor( // Traverse all children.
		osg::NodeVisitor::TRAVERSE_ALL_CHILDREN),
		_name(name) {}

	// This method gets called for every node in the scene
	//   graph. Check each node to see if its name matches
	//   out target. If so, save the node's address.
	virtual void apply(osg::Node& node)
	{
		if (node.getName() == _name)
			_node = &node;

		// Keep traversing the rest of the scene graph.
		traverse(node);
	}

	osg::Node* getNode() { return _node.get(); }

protected:
	std::string _name;
	osg::ref_ptr<osg::Node> _node;
};


Reflection::Reflection(osg::ref_ptr<osg::Group> sceneNode,osg::ref_ptr<osgViewer::View> gameView, osg::ref_ptr<osg::TextureCubeMap> cubeMap)
{
	//osg::Group		*group = new osg::Group();
	int texSize = 1024;
	// Set up the reflection camera
	cameraGroup = new osg::Group();
	reflectionCamera = new osg::Camera();
	reflectionTransform = new osg::MatrixTransform();

	FindNamedNode findObstacles("debugObstaclesGroup");
	sceneNode->accept(findObstacles);
	osg::Node *obstaclesGroup = findObstacles.getNode();

	// Set reflection textures
	FindNamedNode fnn("debugFloorsNode");
	sceneNode->accept(fnn);
	osg::Node *reflectNode = fnn.getNode();



	reflectionCamera->setClearMask(GL_DEPTH_BUFFER_BIT |GL_COLOR_BUFFER_BIT);
	reflectionCamera->setRenderOrder(osg::Camera::PRE_RENDER);
	reflectionCamera->setClearColor(osg::Vec4(0.0f, 0.f, 0.0f, 1.0f));
	reflectionCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	reflectionCamera->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
	reflectionCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	reflectionCamera->setViewport(0, 0, texSize, texSize);

	cameraGroup->setCullCallback(new CUpdateCameraCallback(gameView));

	//beware, that vec4(a,b,c,d) is a plane equation with a,b,c the plane normal and d the plane height
	m_ReflectionClipPlane = new osg::ClipPlane(0, osg::Vec4d(0.0, 0.0, 1.0, 0.0));
	osg::ClipNode* reflectionClipNode = new osg::ClipNode;
	reflectionClipNode->addClipPlane(m_ReflectionClipPlane);
	reflectionClipNode->addChild(obstaclesGroup);
	
	//reflectionCamera->getOrCreateStateSet()->setAttributeAndModes(m_ReflectionClipPlane, osg::StateAttribute::ON);

	/////////test
	//reflectionCamera->setProjectionMatrix(osg::Matrixd::ortho(-3500, 3500, -3500, 3500., 1.f, 600));
	//////////


	reflectionCamera->setClearDepth(1.0);
	reflectionCamera->setCullMask(CAMERA_MASK_MAIN);

	osg::ref_ptr<osg::Texture2D>	texture = new osg::Texture2D();
	texture->setTextureSize(texSize, texSize);
	texture->setInternalFormat(GL_RGBA);
	reflectionCamera->attach((osg::Camera::BufferComponent) osg::Camera::COLOR_BUFFER0, texture);

	//reflectionCamera->getOrCreateStateSet()->setMode(GL_CLIP_PLANE0 + 0, osg::StateAttribute::ON);
	reflectionCamera->getOrCreateStateSet()->setMode(GL_CULL_FACE, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
	reflectionTransform->setMatrix(osg::Matrix::scale(1.0, 1.0, -1.0));


	cameraGroup->addChild(reflectionCamera);
	reflectionCamera->addChild(reflectionTransform);
	reflectionTransform->addChild(reflectionClipNode);
	


	




	reflectNode->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture,
		osg::StateAttribute::ON);

	reflectNode->getOrCreateStateSet()->addUniform(new osg::Uniform("reflectionTex", 0));

	//reflectNode->getOrCreateStateSet()->setAttributeAndModes(shaders::m_allShaderPrograms[shaders::GRID], osg::StateAttribute::ON);

	//reflectionSurface->getOrCreateStateSet()->setTextureAttributeAndModes(1, cubeMap,
	//	osg::StateAttribute::ON);

	//reflectionSurface->getOrCreateStateSet()->addUniform(new osg::Uniform("skyDome",1));

	//reflectionSurface->getOrCreateStateSet()->addUniform(g_cameraEyeU);

}


osg::ref_ptr<osg::Group> Reflection::getReflectionCameraGroup()
{
	return cameraGroup;
}
