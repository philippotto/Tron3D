#include "sampleosgviewer.h"

using namespace troen;

SampleOSGViewer::SampleOSGViewer(osg::GraphicsContext* context /*= nullptr*/)
{
	setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
}

SampleOSGViewer::~SampleOSGViewer()
{
}
