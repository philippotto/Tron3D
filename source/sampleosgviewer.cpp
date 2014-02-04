#include "sampleosgviewer.h"
#include <iostream>

using namespace troen;

SampleOSGViewer::SampleOSGViewer(osg::GraphicsContext* context /*= nullptr*/)
{
	setThreadingModel(osgViewer::ViewerBase::ThreadingModel::ThreadPerContext);
}

SampleOSGViewer::~SampleOSGViewer()
{
}
