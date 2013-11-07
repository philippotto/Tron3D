#pragma once

#include <osgViewer/CompositeViewer>

#include "forwarddeclarations.h"

namespace troen
{

	class SampleOSGViewer : public osgViewer::CompositeViewer
	{
	public:
		SampleOSGViewer(osg::GraphicsContext* context = NULL);
		virtual ~SampleOSGViewer();

		void setCameraManipulator(osgGA::CameraManipulator* cameraManipulator);
		void setSceneData(osg::Group *rootNode);

	private:
		osg::Camera* createCamera(osg::GraphicsContext* context,
								  osgViewer::View* view);
		void resize(int width, int height);

		osg::ref_ptr<osg::Camera>		m_camera;
		osg::ref_ptr<osgViewer::View>	m_view;
	};

}