#pragma once
// OSG
#include <osgViewer/CompositeViewer>

namespace troen
{
	class SampleOSGViewer : public osgViewer::CompositeViewer
	{
	public:
		SampleOSGViewer(osg::GraphicsContext* context = nullptr);
		virtual ~SampleOSGViewer();

	private:
		void resize(int width, int height);

		osg::ref_ptr<osg::Camera>		m_camera;
	};

}