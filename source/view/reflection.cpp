#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <osg/Geode>
#include <osg/ArgumentParser>
#include <osg/Geometry>
#include <osg/Texture2D>
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

/*
**
*/
using namespace troen;

class CUpdateCameraCallback : public osg::NodeCallback
{
public:

	void operator()(osg::Node *node, osg::NodeVisitor *nv)
	{
		if (nv->getVisitorType() == osg::NodeVisitor::CULL_VISITOR)
		{
			osgUtil::CullVisitor	*cv = static_cast<osgUtil::CullVisitor *>(nv);
			osg::Camera				*camera = static_cast<osg::Camera
				*>(node->asGroup()->getChild(0));

			camera->setProjectionMatrix(cv->getCurrentCamera()->getProjectionMatrix());
			camera->setViewMatrix(cv->getCurrentCamera()->getViewMatrix());
		}
		this->traverse(node, nv);
	}
};

/*
**
*/

Reflection::Reflection(osg::ref_ptr<osg::Group> sceneNode,osg::ref_ptr<osg::Node> reflectionSurface)
{
	//osg::Group		*group = new osg::Group();
	int texSize = 1024;
	// Set up the reflection camera
	osg::Group				*cameraGroup = new osg::Group();
	osg::Camera				*reflectionCamera = new osg::Camera();
	osg::MatrixTransform	*reflectionTransform = new osg::MatrixTransform();

	reflectionCamera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	reflectionCamera->setRenderOrder(osg::Camera::PRE_RENDER);
	reflectionCamera->setClearColor(osg::Vec4(0.0f, 1.f, 0.0f, .5f));
	reflectionCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	reflectionCamera->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
	reflectionCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	reflectionCamera->setViewport(0, 0, texSize, texSize);
	reflectionCamera->getOrCreateStateSet()->setAttributeAndModes(new
		osg::ClipPlane(0, osg::Vec4d(0.0, 0.0, 0.0, 0.0)), osg::StateAttribute::ON);
	reflectionTransform->setMatrix(osg::Matrix::scale(1.0, 1.0, -1.0));

	cameraGroup->setCullCallback(new CUpdateCameraCallback());

	cameraGroup->addChild(reflectionCamera);
	reflectionCamera->addChild(reflectionTransform);
	reflectionTransform->addChild(sceneNode);
	//group->addChild(cameraGroup);

	// Set reflection textures
	osg::Texture2D	*texture = new osg::Texture2D();

	texture->setTextureSize(texSize, texSize);
	texture->setInternalFormat(GL_RGB);
	reflectionCamera->attach(osg::Camera::COLOR_BUFFER, texture);
	reflectionSurface->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture,
		osg::StateAttribute::ON);

	reflectionSurface->getOrCreateStateSet()->addUniform(new osg::Uniform("reflectionTex", 0));

		//// Set shaders
		//osg::Program	*program = new osg::Program();
		//osg::Shader		*vertex = new osg::Shader(osg::Shader::VERTEX);
		//osg::Shader		*fragment = new osg::Shader(osg::Shader::FRAGMENT);

		//program->setName("Reflection");
		//vertex->loadShaderSourceFromFile("Reflection.vert");
		//program->addShader(vertex);
		//fragment->loadShaderSourceFromFile("Reflection.frag");
		//program->addShader(fragment);
		//geode->getOrCreateStateSet()->setAttributeAndModes(program,
		//	osg::StateAttribute::ON);

		//// Fragment uniforms
		//geode->getOrCreateStateSet()->addUniform(new osg::Uniform("reflectionTex",
		//	0));

	//// model
	//{
	//	osg::Node				*cow = osgDB::readNodeFile("cow.osg");
	//	osg::MatrixTransform	*cowTransfo = new osg::MatrixTransform();

	//	cowTransfo->setMatrix(osg::Matrix::translate(0, 0, cow->getBound().radius() /
	//		2.0));
	//	cowTransfo->addChild(cow);
	//	scene->addChild(cowTransfo);
	//	group->addChild(scene);
	//}

	//return (group);
}


osg::ref_ptr<osg::Group> Reflection::getReflectionCameraGroup()
{
	return cameraGroup;
}

//int	main(int ac, char **av)
//{
//	osg::ArgumentParser	argumentParser(&ac, av);
//	int					texSize = 1024;
//
//	argumentParser.read("--texture-size", texSize);
//
//	// Create and configure the viewer
//	osgViewer::Viewer	viewer;
//
//	viewer.addEventHandler(new
//		osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));
//	viewer.addEventHandler(new osgViewer::StatsHandler());
//	viewer.addEventHandler(new osgViewer::ThreadingHandler);
//	viewer.addEventHandler(new osgViewer::WindowSizeHandler);
//	viewer.setCameraManipulator(new osgGA::TrackballManipulator());
//	viewer.setUpViewInWindow(100, 100, 800, 600);
//	viewer.setSceneData(createSimpleScene(viewer.getCamera()->getClearColor(),
//		texSize));
//
//	return viewer.run();
//}
