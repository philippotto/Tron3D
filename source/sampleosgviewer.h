#pragma once

#include <osgViewer/CompositeViewer>

namespace troen
{

	class SampleOSGViewer : public osgViewer::CompositeViewer
	{
	public:
		SampleOSGViewer(osg::GraphicsContext* context = NULL);
		virtual ~SampleOSGViewer();
	private:
		void resize(int width, int height);

		osg::ref_ptr<osg::Camera>		m_camera;
	};

}