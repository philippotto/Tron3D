#include "sampleosgviewer.h"

using namespace troen;

SampleOSGViewer::SampleOSGViewer(osg::GraphicsContext* context /*= NULL*/)
{
	setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
}

SampleOSGViewer::~SampleOSGViewer()
{
}
