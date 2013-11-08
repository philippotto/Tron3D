#include "sampleosgviewer.h"

using namespace troen;

SampleOSGViewer::SampleOSGViewer(osg::GraphicsContext* context /*= NULL*/)
{
	setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
}

SampleOSGViewer::~SampleOSGViewer()
{
}

//osg::Camera* SampleOSGViewer::createCamera(osg::GraphicsContext* context,
//										   osgViewer::View* view)
//{
//	const osg::GraphicsContext::Traits* traits = context->getTraits();
//
//	osg::ref_ptr<osg::Camera> camera = view->getCamera();
//	camera->setGraphicsContext(context);
//
//	camera->setClearColor(osg::Vec4(0.2, 0.2, 0.6, 1.0));
//	camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
//	camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width) / static_cast<double>(traits->height), 1.0f, 10000.0f);
//	return camera.release();
//}