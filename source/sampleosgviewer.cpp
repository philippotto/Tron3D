#include "sampleosgviewer.h"
#include <iostream>

using namespace troen;

SampleOSGViewer::SampleOSGViewer(osg::GraphicsContext* context /*= nullptr*/)
{
	CompositeViewer();
	setThreadingModel(osgViewer::ViewerBase::ThreadingModel::SingleThreaded);
}

SampleOSGViewer::~SampleOSGViewer()
{
}
