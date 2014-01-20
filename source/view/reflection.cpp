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
#include <osg/ClipPlane>


//troen
#include "reflection.h"
#include "shaders.h"
#include "skydome.h"

/*
**
*/
using namespace troen;

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
		if (nv->getVisitorType() == osg::NodeVisitor::CULL_VISITOR && false)
		{
			osgUtil::CullVisitor	*cv = static_cast<osgUtil::CullVisitor *>(nv);
			osg::Camera				*camera = static_cast<osg::Camera
				*>(node->asGroup()->getChild(0));

			//camera->setProjectionMatrix(m_gameView->getCamera()->getProjectionMatrix());
			//camera->setViewMatrix(m_gameView->getCamera()->getViewMatrix());

			camera->setViewMatrix(m_reflectionMatrix * m_gameView->getCamera()->getViewMatrix());
			camera->setProjectionMatrix(m_gameView->getCamera()->getProjectionMatrix());

			//_reflectionCamera->accept(*_cv);

		}
		this->traverse(node, nv);
	}
};

/*
**
*/

Reflection::Reflection(osg::ref_ptr<osg::Group> sceneNode,osg::ref_ptr<osg::Node> reflectionSurface, 
	osg::ref_ptr<osgViewer::View> gameView, osg::ref_ptr<osg::TextureCubeMap> cubeMap)
{
	//osg::Group		*group = new osg::Group();
	int texSize = 1024;
	// Set up the reflection camera
	cameraGroup = new osg::Group();
	reflectionCamera = new osg::Camera();
	reflectionTransform = new osg::MatrixTransform();

	reflectionCamera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	reflectionCamera->setRenderOrder(osg::Camera::PRE_RENDER);
	reflectionCamera->setClearColor(osg::Vec4(0.0f, 1.f, 0.0f, .5f));
	reflectionCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	reflectionCamera->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
	reflectionCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF_INHERIT_VIEWPOINT);
	reflectionCamera->setViewport(0, 0, texSize, texSize);
	reflectionCamera->setClearDepth(1.0);

	osg::ref_ptr<osg::Texture2D>	texture = new osg::Texture2D();
	texture->setTextureSize(texSize, texSize);
	texture->setInternalFormat(GL_RGB);
	reflectionCamera->attach(osg::Camera::COLOR_BUFFER, texture);

	//reflectionCamera->setCullMask(_oceanScene->_reflectionSceneMask);
	reflectionCamera->getOrCreateStateSet()->setMode(GL_CLIP_PLANE0 + 0, osg::StateAttribute::ON);
	reflectionCamera->getOrCreateStateSet()->setMode(GL_CULL_FACE, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);

	//reflectionCamera->getOrCreateStateSet()->setAttributeAndModes(new osg::ClipPlane(0, osg::Vec4d(0.0, 0.0, 0.0, 0.0)), osg::StateAttribute::ON);
	//reflectionTransform->setMatrix(osg::Matrix::scale(1.0, 1.0, -1.0));

	reflectionCamera->setCullCallback(new CUpdateCameraCallback(gameView));

	cameraGroup->addChild(reflectionCamera);
	reflectionCamera->addChild(reflectionTransform);
	
	//reflectionCamera->setViewMatrixAsLookAt(osg::Vec3(0.0, 0.0, 10.0), osg::Vec3(0.0, 0.0, 0.0), osg::Vec3(0.0, 0.0, 1.0));
	reflectionTransform->addChild(sceneNode);
	//group->addChild(cameraGroup);

	// Set reflection textures

	reflectionSurface->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture,
		osg::StateAttribute::ON);

	reflectionSurface->getOrCreateStateSet()->addUniform(new osg::Uniform("reflectionTex", 0));


	reflectionSurface->getOrCreateStateSet()->setTextureAttributeAndModes(1, cubeMap,
		osg::StateAttribute::ON);

	reflectionSurface->getOrCreateStateSet()->addUniform(new osg::Uniform("skyDome",1));

	reflectionSurface->getOrCreateStateSet()->addUniform(new osg::Uniform("cameraEye", 1));

}


osg::ref_ptr<osg::Group> Reflection::getReflectionCameraGroup()
{
	return cameraGroup;
}
