#include "sampleosgviewer.h"

using namespace troen;

SampleOSGViewer::SampleOSGViewer(osg::GraphicsContext* context /*= NULL*/)
{
	setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
	setKeyEventSetsDone(0);
}

SampleOSGViewer::~SampleOSGViewer()
{
}
