#include "sampleosgviewer.h"

using namespace troen;

SampleOSGViewer::SampleOSGViewer(osg::GraphicsContext* context /*= NULL*/)
{
	setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
	//double foy, ration, near, far;
	//this->m_camera->getProjectionMatrixAsPerspective(foy, ration, near, far);
	//this->m_camera->setProjectionMatrixAsPerspective(foy, ration, 0.01, far);
}

SampleOSGViewer::~SampleOSGViewer()
{
}
