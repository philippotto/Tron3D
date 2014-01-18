#pragma once
// OSG
#include <osgViewer/View>
#include <osgText/Text>
// troen
#include "../forwarddeclarations.h"
#include "abstractview.h"

namespace troen
{

	class HUDView : public AbstractView
	{
	public:
		HUDView();
		void resize(int width, int height);
		void attachSceneToRadarCamera(osg::Group* scene);
		void setSpeedText(float speed);
        void updateRadarCamera();
        void setTrackNode(osg::Node* trackNode);

	private:
		osg::Camera* createHUD();
		osg::Camera* createRadar();

		osg::ref_ptr<osg::Camera> m_camera;
		osg::ref_ptr<osg::Camera> m_radarCamera;
		osg::ref_ptr<osgGA::NodeTrackerManipulator> m_radarManipulator;
		osg::ref_ptr<osgText::Text> m_speedText;

		osg::Geode* m_savedGeode;
        osg::Node* m_trackNode;
	};
}